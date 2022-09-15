#include <stdbool.h>
#include <glib.h>
#include "command.h"
#include <assert.h>
#include "strextra.h"
#include <string.h>

// pruebacommit
// scommand: Juan y Nacho

struct scommand_s
{
    // Juan
    GList *command;
    char *args_in;
    char *args_out;
};

scommand scommand_new(void)
{
    // Juan
    scommand init;
    init = calloc(1, sizeof(struct scommand_s));
    init->args_in = NULL;
    init->args_out = NULL;
    init->command = NULL;
    assert(init != NULL && scommand_is_empty(init) &&
            scommand_get_redir_in(init) == NULL    &&
            scommand_get_redir_out(init) == NULL);
    return init;
}

scommand scommand_destroy(scommand self)
{
    // Juan
    assert(self != NULL);
    if (self->command != NULL)
    {
        g_list_free_full(self->command, free); /* liberar la memoria de la lista */
        self->command = NULL;
    }
    
    free(self->args_in);
    self->args_in = NULL;
    free(self->args_out);
    self->args_out = NULL;

    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}

void scommand_push_back(scommand self, char *argument)
{
    // Nacho
    assert(self != NULL && argument != NULL);
    // append: Adds a new element on to the end of the list.
    // agrego al final de la lista de commands el argument (nuevo command)
    self->command = g_list_append(self->command, argument);
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self)
{
    // Nacho
    assert(self != NULL && !scommand_is_empty(self));
    free(g_list_nth_data(self->command, 0));
    self->command = g_list_delete_link(self->command, self->command);
}
void scommand_set_redir_in(scommand self, char *filename)
{
    // Nacho
    assert(self != NULL);
    free(self->args_in);
    self->args_in = filename;
}

void scommand_set_redir_out(scommand self, char *filename)
{
    // Nacho
    assert(self != NULL);
    free(self->args_out);
    self->args_out = filename;
}

bool scommand_is_empty(const scommand self)
{
    // Juan
    assert(self != NULL);
    return self->command == NULL;
}
unsigned int scommand_length(const scommand self)
{
    // JUan
    assert(self != NULL);
    unsigned int length = g_list_length(self->command); 
    assert((length == 0)== scommand_is_empty(self));
    return length;
}

char *scommand_front(const scommand self)
{
    // Nacho
    assert(self != NULL && !scommand_is_empty(self));
    char *ret;
    ret = g_list_nth_data(self->command, 0);
    assert(ret != NULL);
    return ret;
}

char *scommand_get_argument(const scommand self, int i)
{
    // Juan
    assert(self != NULL && !scommand_is_empty(self));
    assert(i>=0);
    char *ret = g_list_nth_data(self->command, i);
    return ret;
}

void scommand_to_array(const scommand self, char ** args){

    assert(self != NULL);
    unsigned int length = scommand_length(self);

    for (unsigned int i = 0; i < length; i++)
    {
        args[i] = scommand_get_argument(self,i); 
    }

}

char *scommand_get_redir_in(const scommand self)
{
    // Juan
    assert(self != NULL);
    return self->args_in;
}

char *scommand_get_redir_out(const scommand self)
{
    // Juan
    assert(self != NULL);
    return self->args_out;
}

char *scommand_to_string(const scommand self)
{
    assert(self != NULL);
    GList *list = self->command;
    char *args_in = scommand_get_redir_in(self);
    char *args_out = scommand_get_redir_out(self);
    size_t length = 0;
    char *str=strdup("");

    if (list != NULL)
    {   
        char *list_str;
        for (unsigned int i = 0; i < scommand_length(self); i++)
        {
            length = strlen(str);
            list_str = g_list_nth_data(list, i);
            length = (length + strlen(list_str));
            str = realloc(str, (length + (size_t) 2) * sizeof(char));
            str = strcat(str, list_str);
            str = strcat(str, " ");
        };

        if (args_in != NULL)
        {
            length = strlen(str) + strlen(args_in);
            str = realloc(str, (length + 3) * sizeof(char));
            str = strcat(str, " < ");
            str = strcat(str, args_in);
        };
        if (args_out != NULL)
        {
            length = strlen(str) + strlen(args_out);
            str = realloc(str, (length + 3) * sizeof(char));
            str = strcat(str, " > ");
            str = strcat(str, args_out);
        };
    }



    assert(
        scommand_is_empty(self) ||
        scommand_get_redir_in(self) == NULL ||
        scommand_get_redir_out(self) == NULL ||
        strlen(str) > 0);

    return str;
}

// pipeline: Mili y Tomi
struct pipeline_s
{
    GList *commands_queue;
    bool wait;
};

pipeline pipeline_new(void)
{
    pipeline init;
    init = calloc(1, sizeof(struct pipeline_s));
    init->wait = true;
    init->commands_queue = NULL;
    assert(init != NULL && pipeline_is_empty(init) && pipeline_get_wait(init));

    return init;
}

pipeline pipeline_destroy(pipeline self)
{
    assert(self != NULL);
    if (self->commands_queue != NULL)
    {
        while(self->commands_queue!=NULL) {
            scommand_destroy(pipeline_front(self));
            pipeline_pop_front(self);
        }

        self->commands_queue = NULL;
    }
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}

void pipeline_push_back(pipeline self, scommand sc)
{
    assert(self != NULL && sc != NULL);
    self->commands_queue = g_list_append(self->commands_queue, sc);
    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self)
{
    assert(self != NULL && !pipeline_is_empty(self));
    self->commands_queue = g_list_delete_link(self->commands_queue, self->commands_queue);
}

void pipeline_set_wait(pipeline self, const bool w)
{
    assert(self != NULL);
    self->wait = w;
}

bool pipeline_is_empty(const pipeline self)
{
    assert(self != NULL);
    return self->commands_queue == NULL;
}

unsigned int pipeline_length(const pipeline self)
{
    assert(self != NULL);
    unsigned int length = g_list_length(self->commands_queue);
    assert((length==0) == pipeline_is_empty(self));
    return length;
}

scommand pipeline_front(const pipeline self)
{
    assert(self != NULL && !pipeline_is_empty(self));
    return g_list_nth_data(self->commands_queue, 0);
}

bool pipeline_get_wait(const pipeline self)
{
    assert(self != NULL);
    return self->wait;
}

char *pipeline_to_string(const pipeline self)
{
    // Tomi y Mili
    assert(self != NULL);
    char *str;
    GList *list = self->commands_queue;

    if (list != NULL)
    {
        char*straux;
        str = scommand_to_string(g_list_nth_data(list, 0));
        for (unsigned int i = 1; i < pipeline_length(self); i++)
        {   
            straux=str;
            str = strmerge(straux, " | ");
            straux=str;
            str = strmerge(straux, scommand_to_string(g_list_nth_data(list, i)));
            free(straux);
        };

        if (!self->wait)
        {
            char*straux2=str;

            str = strmerge(straux2, " &");
            free(straux2);
        }
    }
    else {
        str=strmerge("","");
    }

    
    assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(str) > 0);
    return str;
}
