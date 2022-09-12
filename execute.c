#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> //wait
#include <sys/wait.h>  //wait
#include <fcntl.h>
#include <assert.h>

#include "tests/syscall_mock.h"
#include "strextra.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

/* MACROS */
#define reading_tip 0
#define writing_tip 1

static void execute_internal(pipeline apipe)
{
    scommand simple_command = pipeline_front(apipe);
    pipeline_pop_front(apipe);
    builtin_run(simple_command);
}

static void redir_in(char *in)
{
    char *path = in;
    int file = open(path, O_RDONLY, O_CREAT);

    if (file < 0)
    {
        printf("Error, file doesn't exist.");
    }
    else
    {
        int ret_dup = dup2(file, 0);
        assert(ret_dup != -1);
        file = close(file);
        if (file < 0)
        {
            printf("Error while closing the file.");
        }
    }
}
static void redir_out(char *out)
{
    char *path = out;
    int file = open(path, O_WRONLY, O_CREAT);
    assert(file != -1);
    int ret_dup = dup2(file, 1);
    assert(ret_dup != -1);
    file = close(file);
    if (file < 0)
    {
        printf("Error while closing the file.");
    }
}

static void execute_scommand(pipeline apipe)
{
    assert(apipe != NULL);
    scommand simple_command = scommand_new();
    unsigned int length = 0;
    pid_t pid = fork();
    if (pid < 0) // error
    {
        printf("Fork failure, where PID: %d \n", pid);
        exit(EXIT_FAILURE);
    }
    if (pid == 0) // hijo
    {
        simple_command = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        char *in = scommand_get_redir_in(simple_command);
        char *out = scommand_get_redir_out(simple_command);
        length = scommand_length(simple_command);
        char **args = calloc(length, sizeof(char *));
        scommand_to_array(simple_command, args);

        if (in != NULL)
        {
            redir_in(in);
        }
        if (out != NULL)
        {
            redir_out(out);
        }
        execvp(args[0], args);
        printf("error on execvp %d", getpid());
        exit(EXIT_FAILURE);
    }
    else // padre
    {
        if (pipeline_get_wait(apipe))
        {
            wait(NULL);
        }
    }
}

static void execute_multiple_commands(pipeline apipe)
{
    // usar funcion pipe
    pid_t pid;
    int tube[2];
    unsigned int length;

    int ret_pipe = pipe(tube);
    assert(ret_pipe != -1);
    pid = fork();
    if (pid < 0) // error
    {
        printf("fork first child faliure %d \n", pid);
        exit(1);
    }
    else if (pid == 0) // primer hijo hijo
    {
        // creando los argumentos para execvp()
        scommand simple_command = pipeline_front(apipe);
        pipeline_pop_front(apipe);
        length = scommand_length(simple_command);
        char **args = calloc(length, sizeof(char *));
        scommand_to_array(simple_command, args);

        // ver
        int ret_dup = dup2(tube[writing_tip], writing_tip);
        assert(ret_dup != -1);
        int file = close(tube[reading_tip]);
        if (file < 0)
        {
            printf("close file error");
            exit(0);
        }
        execvp(args[0], args);
        printf("error on execvp %d", getpid());
        exit(EXIT_FAILURE);
    }

    else // padre primerizo
    {
        pid = fork();

        if (pid < 0) // error
        {
            printf("fork second child faliure %d \n", pid);
            exit(1);
        }
        else if (pid == 0) // segundo hijo
        {
            scommand simple_command = pipeline_front(apipe);
            pipeline_pop_front(apipe);
            length = scommand_length(simple_command);
            char **args2 = calloc(length, sizeof(char *));
            scommand_to_array(simple_command, args2);
            int ret_dup = dup2(tube[reading_tip], reading_tip);
            assert(ret_dup != -1);
            int file = close(tube[writing_tip]);
            if (file < 0)
            {
                printf("close file error");
                return;
            }
            // chekear que se cierre bien att juan
            execvp(args2[0], args2);
            printf("error on execvp %d", getpid());
            exit(EXIT_FAILURE);
        }
        else // padre finalmente
        {
            if (pipeline_get_wait(apipe)) // si tenemos "&"
            {
                wait(NULL); // un hijo
                wait(NULL); // dos hijos
            }
            close(tube[writing_tip]);
            close(tube[reading_tip]);
        }
    }
}

void execute_pipeline(pipeline apipe)
{
    assert(apipe != NULL);

    // scommand simple_command;
    // unsigned int length, length2;
    //  int file;
    //  char *path;
    // pid_t pid;

    if (!pipeline_is_empty(apipe))
    {
        if (builtin_is_internal(pipeline_front(apipe)))
        {
            execute_internal(apipe);
        }
        else
        {
            if (scommand_front(pipeline_front(apipe)) == NULL)
            {
                exit(1);
            }
            // si el scommand es null salgo sin ejecutar nada

            if (pipeline_length(apipe) == 1)
            {
                execute_scommand(apipe);
            }
            else
            { // dos pipeline
                execute_multiple_commands(apipe);
            }
        }
    }
}