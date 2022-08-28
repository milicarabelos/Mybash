#include <stdbool.h> 
#include <glib.h>
#include "command.h"
#include <assert.h>
#include "strextra.h" 

//pruebacommit
//scommand: Juan y Nacho

struct scommand_s
{
//Juan
    GList * command;
    char * args_in;
    char * args_out;
};

scommand scommand_new(void){
    //Juan
    scommand init;
    init = calloc(1, sizeof(struct scommand_s));
    init->args_in = NULL;
    init->args_out = NULL;
    init->command = NULL;
    g_assert(init != NULL && scommand_is_empty(init));
    return init;
}

scommand scommand_destroy(scommand self){
    //Juan
    g_assert(self != NULL);
    if ( self->command != NULL)
    {
        g_list_free(self->command); /* liberar la memoria de la lista */
        self->command = NULL;
    }

    free(self);
    self = NULL;

    g_assert(self == NULL);
    return self;
}

void scommand_push_back(scommand self, char * argument) {
//Nacho
    assert(self!= NULL && argument !=NULL);
    //append: Adds a new element on to the end of the list.
    //agrego al final de la lista de commands el argument (nuevo command)
    self->command=g_list_append(self->command, argument);
	}


void scommand_pop_front(scommand self) {
    //Nacho
    assert(self!=NULL && !scommand_is_empty(self));
    self->command= g_list_delete_link(self->command,self->command);

}
void scommand_set_redir_in(scommand self, char * filename) {
    //Nacho
    assert(self!=NULL);
    self->args_in=filename;
}

void scommand_set_redir_out(scommand self, char * filename) {
    //Nacho
    assert(self!=NULL);
    self->args_out=filename;
}

bool scommand_is_empty(const scommand self){
    //Juan
    g_assert(self != NULL);
    return self->command != NULL;
}
unsigned int scommand_length(const scommand self){
    //JUan
    g_assert(self!=NULL);
    return g_list_length(self->command); 
}

char * scommand_front(const scommand self) {
    //Nacho
    assert(self!=NULL && !scommand_is_empty(self));
    char * ret;
    ret = g_list_nth_data(self->command,0);
    return ret;
}

char * scommand_get_redir_in(const scommand self){
    //Juan
    g_assert(self != NULL);
    return self->args_in;
}

char * scommand_get_redir_out(const scommand self){
    //Juan
    g_assert(self != NULL);
    return self->args_out;
}

char * scommand_to_string(const scommand self){
    assert(self != NULL);
    GList * list = self->command;
    char * args_in = scommand_get_redir_in(self);
    char * args_out = scommand_get_redir_out(self);

    char * str = NULL;
    if  (list!=NULL) {
        str = scommand_front(self);
        for (unsigned int i = 1; i < scommand_length(self); i++)
        {
            str = strmerge(str, g_list_nth_data(list, i));
            str = strmerge(str, " ");
        };
        
        if (args_in != NULL){
            str = strmerge(str, " < ");
            str = strmerge(str, args_in);
        };
        if (args_out!= NULL) {
            str = strmerge(str, " > ");
            str = strmerge(str, args_out);
        };
    }

    assert(
        scommand_is_empty(self) ||
        scommand_get_redir_in(self)==NULL ||
        scommand_get_redir_out(self)==NULL ||
        strlen(str)>0
    );
    return str;
}


//pipeline: Mili y Tomi
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
    g_assert(init != NULL && pipeline_is_empty(init) && pipeline_get_wait(init));
    
    return init;
}

pipeline pipeline_destroy(pipeline self)
{
    g_assert(self != NULL);
    if ( self->commands_queue != NULL)
    {
        g_list_free(self->commands_queue); /* liberar la memoria de la lista */
        self->commands_queue = NULL;
    }
    free(self);
    self = NULL;    

    g_assert(self == NULL);
    return self;
}    

void pipeline_push_back(pipeline self, scommand sc)
{
    g_assert(self != NULL && sc != NULL);
    self->commands_queue = g_list_append(self->commands_queue, sc);
    g_assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self)
{
    g_assert(self != NULL && !pipeline_is_empty(self));
    self->commands_queue = g_list_delete_link(self->commands_queue,self->commands_queue);
}

void pipeline_set_wait(pipeline self, const bool w)
{
    g_assert(self != NULL);
    self->wait = w;
}

bool pipeline_is_empty(const pipeline self)
{
    g_assert(self != NULL);
    return self->commands_queue != NULL;
}

unsigned int pipeline_length(const pipeline self)
{   g_assert(self!=NULL);
    return g_list_length(self->commands_queue); 
}

scommand pipeline_front(const pipeline self)
{   
    g_assert(self != NULL && !pipeline_is_empty(self));
    return g_list_nth_data(self->commands_queue,0);
}

bool pipeline_get_wait(const pipeline self)
{
    g_assert(self != NULL);
    return self->wait;
}

char * pipeline_to_string(const pipeline self);

