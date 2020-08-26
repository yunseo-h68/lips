#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lips.h"

static int argv_type(const char* argv)
{
	if (strlen(argv) > 0 && argv[0] != '-')
		return SUBCOMMAND;

	if (strlen(argv) > 1 && argv[0] == '-' && argv[1] != '-')
		return OPTION_SHORT;

	if (strlen(argv) > 2 && argv[0] == '-' && argv[1] == '-' && argv[2] != '-')
		return OPTION_LONG;

	return OTHER_TYPE;
}

static char* parse_name(const char* argv)
{
	char* temp = NULL;
	int i = 0;
	int j = 0;

	while (argv[i] == '-') i++;
	j = i;
	while (argv[j] != '=' && strlen(argv) > j) j++;

	j -= i;
	temp = (char*)malloc(sizeof(char) * (j + 1));
	j = 0;

	while (argv[i] != '=' && strlen(argv) != i) {
		temp[j++] = argv[i++];
	}
	temp[j] = '\0';
	return temp;
}

static int parse_option_value(struct lips_option* option, const char* argv)
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

static struct lips_option* get_option(struct lips_args* args, const char* name, int option_type)
{
	int i;

	for (i = 0; i < args->count_options; i++) {
		if (args->options[i]->type == option_type &&
			!strcmp(name, args->options[i]->name)) {
			return args->options[i];
		}
	}

	return NULL;
}

static struct lips_subcommand* get_subcommand(struct lips_args* args, const char* name)
{
	int i;

	for (i = 0; i < args->count_subcommands; i++) {
		if (!strcmp(name, args->subcommands[i]->name)) {
			return args->subcommands[i];
		}
	}

	return NULL;
}

static int add_option(struct lips_args* args, const char* name, int option_type)
{
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
	new_option->name = (char*)malloc(sizeof(char) * strlen(name));
	strcpy(new_option->name, name);

	args->options[args->count_options - 1] = new_option;
	return 0;
}

static int add_subcommand(struct lips_args* args, const char* name)
{
	(args->count_subcommands)++;

	if (args->count_subcommands == 1) {
		args->subcommands = (struct lips_subcommand**)malloc(sizeof(struct lips_subcommand*));
	} else {
		args->subcommands = (struct lips_subcommand**)realloc(args->subcommands, sizeof(struct lips_subcommand*) * (args->count_subcommands));
	}

	struct lips_subcommand* new_subcommand = (struct lips_subcommand*)malloc(sizeof(struct lips_subcommand));
	new_subcommand->is_exist = 0;
	new_subcommand->name = (char*)malloc(sizeof(char) * strlen(name));
	strcpy(new_subcommand->name, name);

	args->subcommands[args->count_subcommands - 1] = new_subcommand;
	return 0;
}

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
	struct lips_subcommand* subcommand = get_subcommand(args, name);
	
	if (subcommand == NULL) {
		return 0;
	}

	return subcommand->is_exist;
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

void lips_parse_args(struct lips_args* args, const int argc, char* argv[])
{
	int i = 0;
	int type = 0;
	char* name = NULL;
	struct lips_option* option_temp = NULL;
	struct lips_subcommand* subcommand_temp = NULL;

	for (i = 1; i < argc; i++) {
		type = argv_type(argv[i]);
		name = parse_name(argv[i]); 
		switch (type) {
			case OPTION_LONG:
			case OPTION_SHORT:
				option_temp = get_option(args, name, type);
				if (option_temp == NULL) {
					break;
				}
				option_temp->is_exist = 1;
				parse_option_value(option_temp, argv[i]);
				break;
			case SUBCOMMAND:
				subcommand_temp = get_subcommand(args, name);
				if (subcommand_temp == NULL) {
					break;
				}
				subcommand_temp->is_exist = 1;
				break;
		}
		free(name);
		name = NULL;
	}
}

struct lips_args* new_lips_args()
{
	struct lips_args* args = (struct lips_args*)malloc(sizeof(struct lips_args));
	args->count_options = 0;
	args->count_options_long = 0;
	args->count_options_short = 0;
	args->count_subcommands = 0;
	args->options = NULL;
	args->subcommands = NULL;
	return args;
}

void delete_lips_args(struct lips_args* args)
{
	int i;

	if (args == NULL) {
		return;
	}
	
	if (args->options != NULL) {
		for (i = 0; i < args->count_options; i++) {
			free(args->options[i]);
		}
		free(args->options);
		args->options = NULL;
	}

	if (args->subcommands != NULL) {
		for (i = 0; i < args->count_subcommands; i++) {
			free(args->subcommands[i]);
		}
		free(args->subcommands);
		args->subcommands = NULL;
	}
	
	free(args);
	args = NULL;
}
