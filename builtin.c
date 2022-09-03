#include <stdbool.h>
#include <assert.h>
#include "command.h"

bool builtin_is_internal(scommand cmd){
//Nacho
/*
 * Indica si el comando alojado en `cmd` es un comando interno
 *
 * REQUIRES: cmd != NULL
 *
 */
}

bool builtin_alone(pipeline p){
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