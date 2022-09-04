#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "builtin.h"

bool builtin_is_internal(scommand cmd)
{
    // Nacho
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "exit") == 0 ||
           strcmp(scommand_front(cmd), "cd") == 0 ||
           strcmp(scommand_front(cmd), "help") == 0;
    // El strcmp: Compara los dos strings y si son iguales retorna 0.
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
    else 
    { 
    if (strcmp(scommand_front(cmd), "cd") == 0)
    {
        if (scommand_get_redir_in(cmd) == NULL)
        {
            chdir("~");
        }
        else
        {
            chdir(scommand_get_redir_in(cmd));
        }
    }
    else
        (strcmp(scommand_front(cmd), "help") == 0)
        {
            // autores en oreden del abecedario
            printf("\n"
                   "My Bash 2022 by Spice Girls\n"

                "Authors:\n"
                "Juan Cruz Pereyra Carrillo\n"
                              "Ignacio Scavuzzo\n"
                                  "Milagros Carabelos\n"
                                      "Tomas Pablo Bazan\n"

                                          "Interanal commands:\n"
                                          "cd : Navega entre directorios del sistema.\n"
                                          "exit : Cierra limpiamente la terminal.\n"
                                          "help : Muestra informacion y ayuda.\n");

                                                                                                                                                      ");
        }
}
}