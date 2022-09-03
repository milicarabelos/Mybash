#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "command.h"

bool builtin_is_internal(scommand cmd) {
    //Nacho
    assert(cmd!=NULL);
    return strcmp(scommand_front(cmd), "exit") == 0 || 
    strcmp(scommand_front(cmd), "cd") == 0 || 
    strcmp(scommand_front(cmd), "help") == 0
    //El strcmp: Compara los dos strings y si son iguales retorna 0.
}

bool builtin_alone(pipeline p){
    //Juan
    assert(p != NULL);
    return pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p));
}

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    if (strcmp(scommand_front(cmd), "exit") == 0) {

    }
    else if (strcmp(scommand_front(cmd), "cd") == 0) {
        if (scommand_get_redir_in(cmd) == NULL){
            chdir("~");
        } else {
            chdir(scommand_get_redir_in(cmd));
        }
    }
    else (strcmp(scommand_front(cmd), "help") == 0) {
        //autores en oreden del abecedario
        printf("
            My Bash 2022
            by Spice Girls B)

            Authors:
            Juan Cruz Pereyra Carrillo
            Ignacio Scavuzzo
            Milagros Carabelos
            Tomas Pablo Bazan


            Interanal commands:
            cd: Navega entre directorios del sistema.
            exit: Cierra limpiamente la terminal.
            help: Muestra informacion y ayuda.


        ");
    }
}