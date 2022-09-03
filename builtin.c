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

//Los dos B)
/*
 * Ejecuta un comando interno
 *
 * REQUIRES: {builtin_is_internal(cmd)}
 *
 */
}