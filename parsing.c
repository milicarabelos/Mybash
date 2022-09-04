#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "parsing.h"
#include "parser.h"
#include "command.h"


static scommand parse_scommand(Parser p)
{
    /* Devuelve NULL cuando hay un error de parseo */
    return NULL;
}

pipeline parse_pipeline(Parser p)
{
    assert(p != NULL && !parser_at_eof(p));
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe = true;
    bool wait,garbage;

    cmd = parse_scommand(p);
    error = (cmd == NULL); /* Comando inválido al empezar */

    while (!error && another_pipe)
    {
        pipeline_push_back(result, cmd);
        cmd = parse_scommand(p);
        another_pipe = (cmd != NULL);
    }

    parser_op_background(p, &wait);

    pipeline_set_wait(result,!(wait));//si hay un & no espera     
    /* Opcionalmente un OP_BACKGROUND al final */
    /*
     *
     * COMPLETAR
     *
     */

    /* Tolerancia a espacios posteriores */
    parser_skip_blanks(p);
    /* Consumir todo lo que hay inclusive el \n */
    parser_garbage(p,&garbage);
    /* Si hubo error, hacemos cleanup */

    if (garbage){
        char * str = strdup("");
        str = parser_last_garbage(p);

        printf("%s", str);
    }

    if (parser_at_eof(p))
    {
        parser_destroy(p);
    }

    return result; // MODIFICAR
}
