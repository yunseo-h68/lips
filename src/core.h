#ifndef LIPS_CORE_H
#define LIPS_CORE_H

#define SAFE_FREE(p) {if(p!=NULL){free(p);p=NULL;}}

#define OTHER_TYPE   3
#define SUBCOMMAND   2
#define OPTION_LONG  1
#define OPTION_SHORT 0

struct lips_option {
	int is_exist;
	int type;
	char* name;
	char* value;
};

struct lips_subcommand {
	int is_exist;
	char* name;
};

struct lips_args {
	int count_options;
	int count_options_long;
	int count_options_short;
	int count_subcommands;
	int count_other_args;

	char* subcommand;
	char** other_args;

	struct lips_option** options;
	struct lips_subcommand** subcommands;
};

static int set_subcommand(struct lips_args* args, const char* name);

int argv_type(const char* argv);
int type_is_option(const int type);

char* parse_name(const char* argv, const int type);
int parse_option_value(struct lips_option* option, const char* argv);

struct lips_option* get_option(struct lips_args* args, const char* name, int option_type);
struct lips_subcommand* get_subcommand(struct lips_args* args, const char* name);

int add_option(struct lips_args* args, const char* name, int option_type);
int add_subcommand(struct lips_args* args, const char* name);
int add_other_args(struct lips_args* args, const char* name);

void parse_args(struct lips_args* args, const int argc, char* argv[]);

int free_subcommands(struct lips_args* args);
int free_options(struct lips_args* args);
int free_other_args(struct lips_args* args);

#endif
