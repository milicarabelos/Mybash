#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

static void show_prompt(void)
{
    printf("mybash> ");
    fflush(stdout);
}
/*
int check_for_EOF()
{
    if (feof(stdin))
        return 1;
    int c = getc(stdin);
    if (c == EOF)
        return 1;
    ungetc(c, stdin);
} */

int main(int argc, char *argv[])
{
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit)
    {
        quit = parser_at_eof(input);
        if (!quit)
        {
            show_prompt();
            pipe = parse_pipeline(input);
            if(pipe != NULL){execute_pipeline(pipe);}
        }
        else
        {
            printf("\n");
        }
        /* Hay que salir luego de ejecutar? */
        // spoiler no, hay que salir cuando hagamos el ctrl+d
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}
