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
    scommand simple_command = pipeline_front(apipe);
    unsigned int length = scommand_length(simple_command);
    char *args[length];
    int file;
    char *path;

    if (pipeline_length(apipe) == 1)
    {
        pid_t pid;
        pid = fork();

        if (pid < 0) // error
        {
            printf("Fork failure, where PID: %d \n", pid);
            exit(1);
        }
        if (pid == 0) // hijo
        {
            char *in = scommand_get_redir_in(simple_command);
            char *out = scommand_get_redir_out(simple_command);

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
        }
    }
}