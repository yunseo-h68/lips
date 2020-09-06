#include <stdlib.h>
#include <string.h>
#include "core.h"

int lips_is_exist_option(struct lips_args* args, const char* name)
{
	struct lips_option* option = get_option(args, name, OPTION_SHORT);

	if (option == NULL) {
		return 0;
	}

	return option->is_exist;
}

int lips_is_exist_option_long(struct lips_args* args, const char* name)
{
	struct lips_option* option = get_option(args, name, OPTION_LONG);
	
	if (option == NULL) {
		return 0;
	}

	return option->is_exist;
}

int lips_is_exist_subcommand(struct lips_args* args, const char* name)
{
	RETURN_ERROR_INT(args == NULL || name == NULL);
	return args->subcommand != NULL && !strcmp(name, args->subcommand);
}

int lips_add_option(struct lips_args* args, const char* name)
{
	return add_option(args, name, OPTION_SHORT);
}

int lips_add_option_long(struct lips_args* args, const char* name)
{
	return add_option(args, name, OPTION_LONG);
}

int lips_add_subcommand(struct lips_args* args, const char* name)
{
	return add_subcommand(args, name);
}

char* lips_get_option_value(struct lips_args* args, const char* name)
{
	struct lips_option* option = get_option(args, name, OPTION_SHORT);
	return option == NULL ? NULL : option->value;
}

char* lips_get_option_long_value(struct lips_args* args, const char* name)
{
	struct lips_option* option = get_option(args, name, OPTION_LONG);
	return option == NULL ? NULL : option->value;
}

char** lips_get_other(struct lips_args* args)
{
	RETURN_ERROR_POINTER(args == NULL);
	return args->other_args;
}

int lips_parse_args(struct lips_args* args, const int argc, char* argv[])
{
	return parse_args(args, argc, argv);
}

struct lips_args* new_lips_args()
{
	struct lips_args* args = (struct lips_args*)malloc(sizeof(struct lips_args));
	
	args->count_options = 0;
	args->count_options_long = 0;
	args->count_options_short = 0;
	args->count_subcommands = 0;
	args->count_other_args = 0;
	args->subcommand = NULL;
	args->other_args = NULL;
	args->options = NULL;
	args->subcommands = NULL;
	return args;
}

void delete_lips_args(struct lips_args* args)
{
	if (args == NULL) {
		return;
	}
	free_options(args);
	free_subcommands(args);
	free_other_args(args);
	SAFE_FREE(args)
}
