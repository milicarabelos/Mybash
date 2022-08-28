#include <stdbool.h> 
#include <glib-2.0/glib.h> //revisar esto
#include "command.h"
#include <assert.h>
//pruebacommit
//scommand: Juan y Nacho
struct scommand_s
{
//Juan
};

scommand scommand_new(void);
//Juan
scommand scommand_destroy(scommand self);
//Juan
void scommand_push_back(scommand self, char * argument) {
//Nacho
    assert(self!= NULL && argument !=NULL);
    //append: Adds a new element on to the end of the list.
    //agrego al final de la lista de commands el argument (nuevo command)
    self->command=g_list_append(self->command, argument);
	}

void tail(GList * list) {
//Nacho
    assert(list!=NULL);
    //glist no tiene definido el tipo tail, y como no podemos acceder a la representacion interna
    //hacemos un "tail"
    GList * aux; 
    aux=list;//por ahora sino no me deja no incializarlo, y si lo incializo en NULL hay problemas
    for(unsigned int i=1;i<g_list_length(list);i++) {
        aux=g_list_append(aux,(g_list_nth_data(list,i)));
    }
    list=g_list_copy(aux);
    g_list_free(aux);
//Podria ir dentro de scommand_pop_front sin necesidad de definirla como otra funcion pero la modularizamos por si se necesita de su uso
}

void scommand_pop_front(scommand self) { //observacion 1: requiere de una implementacion de la funcion tail() de listas que no esta en la libreria, por eso se agrega en el .h tmb.
    //Nacho
    assert(self!=NULL && !scommand_is_empty(self));
    tail(self->command);
//observacion2: verificar si se puede implementar sin necesidad de tail. Es decir si existe una funcion que elimine elementos

}
void scommand_set_redir_in(scommand self, char * filename) {
    //Nacho
    assert(self!=NULL);
    self->args_in=filename;
}

void scommand_set_redir_out(scommand self, char * filename);
//Nacho
bool scommand_is_empty(const scommand self);
//Juan
unsigned int scommand_length(const scommand self);
//Juan
char * scommand_front(const scommand self);
//Nacho
char * scommand_get_redir_in(const scommand self);
//Juan
char * scommand_get_redir_out(const scommand self);
//Juan
char * scommand_to_string(const scommand self);
//Juan y Nacho



//pipeline: Mili y Tomi
struct pipeline_s
{
    GSList *commands_queue;
    bool wait;
};

pipeline pipeline_new(void);

pipeline pipeline_destroy(pipeline self);

void pipeline_push_back(pipeline self, scommand sc);

void pipeline_pop_front(pipeline self);

void pipeline_set_wait(pipeline self, const bool w);

bool pipeline_is_empty(const pipeline self);

unsigned int pipeline_length(const pipeline self);

scommand pipeline_front(const pipeline self);

bool pipeline_get_wait(const pipeline self);

char * pipeline_to_string(const pipeline self);

