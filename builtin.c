#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"

bool builtin_is_internal(scommand cmd)
{
    // Nacho
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "exit") == 0 ||
           strcmp(scommand_front(cmd), "cd") == 0 ||
           strcmp(scommand_front(cmd), "help") == 0
}

bool builtin_alone(pipeline p)
{
    // Juan
    assert(p != NULL);
    return pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p));
}

void builtin_run(scommand cmd)
{
    assert(builtin_is_internal(cmd));
    if (strcmp(scommand_front(cmd), "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(scommand_front(cmd), "cd") == 0)
    {
        if (get_scommand_argument(cmd, 2) != NULL)
        {
            printf("cd: Too many arguments");
        }

        arg = get_scommand_argument(cmd, 1);
        if (arg == NULL)
        {
            chdir("~");
        }
        else
        {
            chdir(arg);
        }
    }
    else
        (strcmp(scommand_front(cmd), "help") == 0)
        {
            printf("
                My Bash 2022 by Spice Girls B)\n\n
                Authors:
                    * Tomas Pablo Bazan\n
                    * Milagros Carabelos\n
                    * Juan Cruz Pereyra Carrillo\n
                    * Ignacio Scavuzzo\n\n
                Interanal commands : 
                    cd : Navigate the file system in your device\n
                    exit : Clean close the terminal\n
                    help : Show this text!\n

                                                                                                                                                      ");
        }
}