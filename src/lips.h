#ifndef LIPS_LIPS_H
#define LIPS_LIPS_H

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

	struct lips_option** options;
	struct lips_subcommand** subcommands;
};

static int argv_type(const char* argv);
static char* parse_name(const char* argv);
static int parse_option_value(struct lips_option* option, const char* argv);
static struct lips_option* get_option(struct lips_args* args, const char* name, int option_type);
static struct lips_subcommand* get_subcommand(struct lips_args* args, const char* name);
static int add_option(struct lips_args* args, const char* name, int option_type);
static int add_subcommand(struct lips_args* args, const char* name);

int lips_is_exist_option(struct lips_args* args, const char* name);
int lips_is_exist_option_long(struct lips_args* args, const char* name);
int lips_is_exist_subcommand(struct lips_args* args, const char* name);
int lips_add_option(struct lips_args* args, const char* name);
int lips_add_option_long(struct lips_args* args, const char* name);
int lips_add_subcommand(struct lips_args* args, const char* name);
void lips_parse_args(struct lips_args* args, const int argc, char* argv[]);

struct lips_args* new_lips_args();
void delete_lips_args(struct lips_args* args);

#endif
