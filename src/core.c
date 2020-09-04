#include <stdlib.h>
#include <string.h>
#include "core.h"

static int set_subcommand(struct lips_args* args, const char* name)
{
	args->subcommand = (char*)malloc(strlen(name));
	strcpy(args->subcommand, name);
	return 0;
}

int argv_type(const char* argv)
{
	if (strlen(argv) > 0 && argv[0] != '-')
		return SUBCOMMAND;

	if (strlen(argv) > 1 && argv[0] == '-' && argv[1] != '-')
		return OPTION_SHORT;

	if (strlen(argv) > 2 && argv[0] == '-' && argv[1] == '-' && argv[2] != '-')
		return OPTION_LONG;

	return OTHER_TYPE;
}

int type_is_option(const int type)
{
	return type == OPTION_SHORT || type == OPTION_LONG;
}

char* parse_name(const char* argv, const int type)
{
	char* temp = NULL;
	int i = 0;
	int j = 0;

	while (type_is_option(type) && argv[i] == '-') i++;
	j = i;

	while (strlen(argv) > j) {
		if (type_is_option(type) && argv[j] == '=') {
			break;
		}
		j++;
	}

	j -= i;
	temp = (char*)malloc(sizeof(char) * (j + 1));
	j = 0;

	while (strlen(argv) != i) {
		if (type_is_option(type) && argv[i] == '=') {
			break;
		}
		temp[j++] = argv[i++];
	}
	temp[j] = '\0';

	return temp;
}

int parse_option_value(struct lips_option* option, const char* argv)
{
	int i = 0;
	int j = 0;

	while (argv[i] != '=' && strlen(argv) > i) i++;
	j = i;
	while (strlen(argv) > j) j++;

	if (strlen(argv) != i) {
		j -= i;
		option->value = (char*)malloc(sizeof(char) * (j + 1));
		j = 0;
		i++;
		while (strlen(argv) != i) {
			option->value[j++] = argv[i++];
		}
		option->value[j] = '\0';
	}

	return 0;
}

struct lips_option* get_option(struct lips_args* args, const char* name, int option_type)
{
	int i;

	if (args == NULL || name == NULL || strlen(name) == 0) {
		return NULL;
	}

	for (i = 0; i < args->count_options; i++) {
		if (args->options[i]->type == option_type &&
			!strcmp(name, args->options[i]->name)) {
			return args->options[i];
		}
	}

	return NULL;
}

struct lips_subcommand* get_subcommand(struct lips_args* args, const char* name)
{
	int i;

	if (args == NULL || name == NULL || strlen(name) == 0) {
		return NULL;
	}

	for (i = 0; i < args->count_subcommands; i++) {
		if (!strcmp(name, args->subcommands[i]->name)) {
			return args->subcommands[i];
		}
	}

	return NULL;
}

int add_option(struct lips_args* args, const char* name, int option_type)
{
	if (args == NULL || strlen(name) == 0) {
		return -1;
	}

	(args->count_options)++;

	if (option_type == OPTION_LONG) {
		(args->count_options_long)++;
	} else {
		(args->count_options_short)++;
	}

	if (args->count_options == 1) {
		args->options = (struct lips_option**)malloc(sizeof(struct lips_option*));
	} else {
		args->options = (struct lips_option**)realloc(args->options, sizeof(struct lips_option*) * (args->count_options));
	}

	struct lips_option* new_option = (struct lips_option*)malloc(sizeof(struct lips_option));
	new_option->is_exist = 0;
	new_option->type = option_type;
	new_option->name = (char*)malloc(strlen(name) + 1);
	strcpy(new_option->name, name);
	new_option->value = NULL;

	args->options[args->count_options - 1] = new_option;
	return 0;
}

int add_subcommand(struct lips_args* args, const char* name)
{
	if (args == NULL || name == NULL || strlen(name) == 0) {
		return -1;
	}

	(args->count_subcommands)++;

	if (args->count_subcommands == 1) {
		args->subcommands = (struct lips_subcommand**)malloc(sizeof(struct lips_subcommand*));
	} else {
		args->subcommands = (struct lips_subcommand**)realloc(args->subcommands, sizeof(struct lips_subcommand*) * (args->count_subcommands));
	}

	struct lips_subcommand* new_subcommand = (struct lips_subcommand*)malloc(sizeof(struct lips_subcommand));
	new_subcommand->is_exist = 0;
	new_subcommand->name = (char*)malloc(strlen(name) + 1);
	strcpy(new_subcommand->name, name);

	args->subcommands[args->count_subcommands - 1] = new_subcommand;
	return 0;
}

int add_other_args(struct lips_args* args, const char* name)
{
	if (args == NULL || name == NULL || strlen(name) == 0) {
		return -1;
	}

	(args->count_other_args)++;

	if (args->count_other_args == 1) {
		args->other_args = (char**)malloc(sizeof(char*));
	} else {
		args->other_args = (char**)realloc(args->other_args, sizeof(char*) * (args->count_other_args));
	}

	args->other_args[args->count_other_args - 1] = (char*)malloc(strlen(name) + 1);
	strcpy(args->other_args[args->count_other_args - 1], name);

	return 0;
}

void parse_args(struct lips_args* args, const int argc, char* argv[])
{
	int i = 0;
	int type = 0;
	int check_subcommand = 0;
	char* name = NULL;
	struct lips_option* option_temp = NULL;
	struct lips_subcommand* subcommand_temp = NULL;

	for (i = 1; i < argc; i++) {
		type = argv_type(argv[i]);
		name = parse_name(argv[i], type);

		switch (type) {
			case OPTION_LONG:
			case OPTION_SHORT:
				option_temp = get_option(args, name, type);
				if (option_temp != NULL) {
					option_temp->is_exist = 1;
					parse_option_value(option_temp, argv[i]);
					break;
				}
				add_other_args(args, argv[i]);
				break;
			case SUBCOMMAND:
				if (!check_subcommand) {
					check_subcommand = 1;
					subcommand_temp = get_subcommand(args, name);
					if (subcommand_temp != NULL) {
						set_subcommand(args, name);
						subcommand_temp->is_exist = 1;
						break;
					}
				}
				add_other_args(args, argv[i]);
				break;
		}
		SAFE_FREE(name)
	}
}

int free_subcommands(struct lips_args* args)
{
	int i = 0;

	SAFE_FREE(args->subcommand);
	for (i = 0; i < args->count_subcommands; i++) {
		SAFE_FREE(args->subcommands[i]->name)
		SAFE_FREE(args->subcommands[i])
	}
	SAFE_FREE(args->subcommands)
	args->count_subcommands = 0;
	
	return 0;
}

int free_options(struct lips_args* args)
{
	int i = 0;

	for (i = 0; i < args->count_options; i++) {
		SAFE_FREE(args->options[i]->name)
		SAFE_FREE(args->options[i]->value)
		SAFE_FREE(args->options[i])
	}
	SAFE_FREE(args->options)

	args->count_options = 0;
	args->count_options_long = 0;
	args->count_options_short = 0;

	return 0;
}

int free_other_args(struct lips_args* args)
{
	int i = 0;
	
	for (i = 0; i <args->count_other_args; i++) {
		SAFE_FREE(args->other_args[i])
	}
	SAFE_FREE(args->other_args)
	args->count_other_args = 0;
	
	return 0;
}
