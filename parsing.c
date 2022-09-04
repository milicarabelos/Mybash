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
    // simple_command for save the scommand, command_text for analize the string,
    // argument_type for know wicht tipe of input is,
    assert(p != NULL && !parser_at_eof(p));
    scommand simple_command = scommand_new();

    char *command_text = strdup("");
    arg_kind_t argument_type;

    parser_skip_blanks(p);
    command_text = parser_next_argument(p, &argument_type);

    if (command_text == NULL)
    {
        free(simple_command);
        simple_command = NULL;
    }
    

    while (command_text != NULL)
    {
        if (argument_type == ARG_NORMAL)
        {
            scommand_push_back(simple_command, command_text);
        }
        else if (argument_type == ARG_INPUT)
        {
            scommand_set_redir_in(simple_command, command_text);
        }
        else if (argument_type == ARG_OUTPUT)
        {
            scommand_set_redir_out(simple_command, command_text);
        }
        command_text = strdup("");
        command_text = parser_next_argument(p, &argument_type);
    }

    free(command_text);
    command_text = NULL;
    return simple_command;
}

pipeline parse_pipeline(Parser p)
{
    assert(p != NULL && !parser_at_eof(p));
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe = true;
    bool wait, garbage;

    cmd = parse_scommand(p);
    error = (cmd == NULL); /* Comando inválido al empezar */

    while (!error && another_pipe)
    {
        pipeline_push_back(result, cmd);
        cmd = parse_scommand(p);
        parser_op_pipe(p,&another_pipe);
    }

    parser_op_background(p, &wait);

    pipeline_set_wait(result, !(wait)); // si hay un & no espera
    /* Opcionalmente un OP_BACKGROUND al final */
    /*
     *
     * COMPLETAR
     *
     */

    /* Tolerancia a espacios posteriores */
    parser_skip_blanks(p);
    /* Consumir todo lo que hay inclusive el \n */
    parser_garbage(p, &garbage);
    /* Si hubo error, hacemos cleanup */

    if (garbage)
    {
        char *str = strdup("");
        str = parser_last_garbage(p);

        printf("%s", str);
    }

    if (parser_at_eof(p))
    {
        parser_destroy(p);
    }

    if (error)
    {
        pipeline_destroy(result);
        result = NULL;
    }

    return result; // MODIFICAR
}
