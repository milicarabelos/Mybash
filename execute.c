#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> //wait
#include <sys/wait.h>  //wait
#include <fcntl.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

void execute_pipeline(pipeline apipe)
{
    if (pipeline_length(apipe) == 1)
    {
        pid_t pid;
        pid = fork();

        if (pid < 0) // error
        {
            printf("fork faliure %d \n", pid);
            return exit(1);
        }
        else if (pid == 0) // hijo
        {
            scommand simple_command = pipeline_front(apipe);
            unsigned int lenght = scommand_length(simple_command);
            const char **args[lenght];
            scommand_list_to_array(simple_command, args);

            if(//si tiene archivo de entrada)
            {   
                file =
                dup2(file, 0);
            }
            if(//si tiene archivo de salida)
            {   
                file = 
                dup2(file, 1);
            }

            execvp(args[0], args);
        }
        else // padre
        {   
            if(pipeline_get_wait(apipe)){
            wait(NULL);
            }
        }
    }
    else{ //dos pipeline
        
        //usar funcion pipe 
    }
}