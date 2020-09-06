#include <stdio.h>
#include "../include/lips.h"

void add_options(struct lips_args* args);
void add_subcommands(struct lips_args* args);
void print_help();
void print_other(struct lips_args* args);
void print_option_value(struct lips_args* args, const char* name);

int main(int argc, char* argv[]) 
{
	int i = 0;
	struct lips_args* args = new_lips_args();
	add_options(args);
	add_subcommands(args);
	lips_parse_args(args, argc, argv);

	if (argc == 1) {
		print_help();
		return 0;
	}

	// Subcommands
	if (lips_is_exist_subcommand(args,"hello")) {
		printf("hello\n");
		delete_lips_args(args);
		return 0;
	} else if (lips_is_exist_subcommand(args, "world")) {
		printf("world\n");
		delete_lips_args(args);
		return 0;
	} else if (lips_is_exist_subcommand(args, "test")) {
		printf("#TEST\n");
		print_other(args);
		printf("Subcommand : %s\n", args->subcommand);
		delete_lips_args(args);
		return 0;
	}

	// Options
	if (lips_is_exist_option(args, "h") || lips_is_exist_option_long(args, "help")) {
		print_help();
	} else if (lips_is_exist_option(args, "v") || lips_is_exist_option_long(args, "version")) {
		printf("version 1.0.0\n");
	} else if (lips_is_exist_option_long(args, "echo")) {
		print_option_value(args, "echo");
	}

	delete_lips_args(args);
	return 0;
}

void add_options(struct lips_args* args)
{
	// Add -h,--help Option
	lips_add_option(args, "h");
	lips_add_option_long(args, "help");

	// Add -v,--version Option
	lips_add_option(args, "v");
	lips_add_option_long(args, "version");
	
	// Add --echo Option
	lips_add_option_long(args, "echo");
}

void add_subcommands(struct lips_args* args)
{
	// Add hello Subcommand
	lips_add_subcommand(args, "hello");
	
	// Add world Subcommand
	lips_add_subcommand(args, "world");

	// Add test Subcommand
	lips_add_subcommand(args, "test");
}

void print_help() 
{
	printf("example | example of lips\n\n");
	printf("Usage:\n\texample [OPTIONS] [SUBCOMMANDS]\n\n");
	printf("Options:\n");
	printf("\t-h, --help : Display this information\n");
	printf("\t-v, --version : Display version\n");
	printf("\t    --echo=[STRING] : Display [STRING]\n");
	printf("\n");
	printf("Subcommands:\n");
	printf("\thello : print 'Hello'\n");
	printf("\tworld : print 'World'\n");
	printf("\ttest  : print subcommand and other arguments\n\n");
}

void print_other(struct lips_args* args)
{
	int i = 0;
	printf("-other args-\n");
	printf("--use lips_get_other()\n");
	char** other = lips_get_other(args);
	for (i = 0; i < args->count_other_args; i++) {
		printf("%s\n", other[i]);
	}
	printf("--or access other_args field\n");
	for (i = 0; i < args->count_other_args; i++) {
		printf("%s\n", args->other_args[i]);
	}
	printf("\n");
}

void print_option_value(struct lips_args* args, const char* name) 
{
	char* str = lips_get_option_long_value(args, name);
	if (str == NULL) {
		print_help();
		return;
	}
	printf("%s\n", str);
}
