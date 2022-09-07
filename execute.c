#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> //wait
#include <sys/wait.h>  //wait
#include <fcntl.h>

#include "strextra.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

void execute_pipeline(pipeline apipe)
{
    scommand simple_command;
    unsigned int length;
    int file;
    char *path;
    pid_t pid;
    int tube[2];

    if (pipeline_length(apipe) == 1)
    {
        pid = fork();

        if (pid < 0) // error
        {
            printf("Fork failure, where PID: %d \n", pid);
            exit(1);
        }
        if (pid == 0) // hijo
        {
            simple_command = pipeline_front(apipe);
            char *in = scommand_get_redir_in(simple_command);
            char *out = scommand_get_redir_out(simple_command);

            length = scommand_length(simple_command);
            char *args[length];
            scommand_to_array(simple_command, args);

            if (in != NULL)
            {
                path = strmerge("./", in);
                file = open(path, O_RDONLY);
                if (file < 0)
                {
                    printf("Error, file doesn't exist.");
                }
                else
                {
                    dup2(file, 0);
                    file = close(file);
                    if (file < 0)
                    {
                        printf("Error while closing the file.");
                    }
                }
            }
            else if (out != NULL)
            {
                path = strmerge("./", out);
                file = open(path, O_CREAT);
                dup2(file, 1);
                file = close(file);
                if (file < 0)
                {
                    printf("Error while closing the file.");
                }
                execvp(args[0], args);
            }
            else // padre
            {
                if (pipeline_get_wait(apipe))
                {
                    wait(NULL);
                }
            }
        }
    }
    else
    { // dos pipeline

        // usar funcion pipe
        pid = fork();
        pipe(tube);

        if (pid < 0) // error
        {
            printf("fork first child faliure %d \n", pid);
            exit(1);
        }
        else if (pid == 0) // primer hijo hijo
        {
            // creando los argumentos para execvp()
            scommand simple_command = pipeline_front(apipe);
            unsigned int lenght = scommand_length(simple_command);
            char *args[lenght];
            scommand_to_array(simple_command, args);

            //ver
            dup2(tube[1], 1);
            close(tube[1]);
            execvp(args[0], args);
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
                pipeline_pop_front(apipe);
                scommand simple_command = pipeline_front(apipe);
                unsigned int lenght = scommand_length(simple_command);
                char *args2[lenght];
                scommand_to_array(simple_command, args2);
 
                dup2(tube[0], 0);
                close(tube[0]);
                execvp(args2[0], args2);

            }
            else //padre finalmente
            {
                if (!pipeline_get_wait(apipe)) // si tenemos "&"
                {
                    wait(NULL); // un hijo
                    wait(NULL); // dos hijos
                }
            }
            close(tube[0]);
            close(tube[1]);
        }
    }
}