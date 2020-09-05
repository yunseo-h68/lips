#ifndef LIPS_LIPS_H
#define LIPS_LIPS_H

#include "core.h"

int lips_is_exist_option(struct lips_args* args, const char* name);
int lips_is_exist_option_long(struct lips_args* args, const char* name);
int lips_is_exist_subcommand(struct lips_args* args, const char* name);
int lips_add_option(struct lips_args* args, const char* name);
int lips_add_option_long(struct lips_args* args, const char* name);
int lips_add_subcommand(struct lips_args* args, const char* name);
char* lips_get_option_value(struct lips_args* args, const char* name);
char* lips_get_option_long_value(struct lips_args* args, const char* name);
char* lips_get_other(struct lips_args* args);
int lips_parse_args(struct lips_args* args, const int argc, char* argv[]);

struct lips_args* new_lips_args();
void delete_lips_args(struct lips_args* args);

#endif
