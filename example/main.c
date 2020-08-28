#include <stdio.h>
#include "../include/lips.h"

struct lips_args* init(int argc, char* argv[])
{
	struct lips_args* args = new_lips_args();
	
	// Add -h,--help Option
	lips_add_option(args, "h");
	lips_add_option_long(args, "help");

	// Add -v,--version Option
	lips_add_option(args, "v");
	lips_add_option_long(args, "version");
	
	// Add --echo Option
	lips_add_option_long(args, "echo");
	
	// Add hello Subcommand
	lips_add_subcommand(args, "hello");
	
	// Add world Subcommand
	lips_add_subcommand(args, "world");

	lips_parse_args(args, argc, argv);
	return args;
}

void print_help() {
	printf("example | example of lips\n\n");
	printf("Usage:\n\texample [OPTIONS] [SUBCOMMANDS]\n\n");
	printf("Options:\n");
	printf("\t-h, --help : Display this information\n");
	printf("\t-v, --version : Display version\n");
	printf("\t    --echo=[STRING] : Display [STRING]\n");
	printf("\n");
	printf("Subcommands:\n");
	printf("\thello : print 'Hello'\n");
	printf("\tworld : print 'World'\n\n");
}

void print_version() {
	printf("version 0.1.3\n");
}

void print_hello() {
	printf("Hello\n");
}

void print_world() {
	printf("World\n");
}

void option_echo(struct lips_args* args) {
	char* str = lips_get_option_long_value(args, "echo");
	if (str == NULL) {
		print_help();
		return;
	}
	printf("%s\n", str);
}

int main(int argc, char* argv[]) {
	struct lips_args* args = init(argc, argv);

	if (argc == 1) {
		print_help();
		return 0;
	}

	// Subcommands
	if (lips_is_exist_subcommand(args,"hello")) {
		print_hello();
		return 0;
	} else if (lips_is_exist_subcommand(args, "world")) {
		print_world();
		return 0;
	}

	// Options
	if (lips_is_exist_option(args, "h") || lips_is_exist_option_long(args, "help")) {
		print_help();
	} else if (lips_is_exist_option(args, "v") || lips_is_exist_option_long(args, "version")) {
		print_version();
	} else if (lips_is_exist_option_long(args, "echo")) {
		option_echo(args);
	}

	delete_lips_args(args);
	return 0;
}
