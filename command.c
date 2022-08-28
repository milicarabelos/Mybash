#include <stdbool.h> 
#include <glib-2.0/glib.h> //revisar esto
#include "command.h"
#include <assert.h>

//scommand: Juan y Nacho
struct scommand_s
{
//Juan
};

scommand scommand_new(void);
//Juan
scommand scommand_destroy(scommand self);
//Juan
void scommand_push_back(scommand self, char * argument);
//Nacho
void scommand_pop_front(scommand self);
//Nacho
void scommand_set_redir_in(scommand self, char * filename);
//Nacho
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
