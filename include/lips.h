#ifndef LIPS_H
#define LIPS_H

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

int lips_is_exist_option(struct lips_args* args, const char* name);
int lips_is_exist_option_long(struct lips_args* args, const char* name);
int lips_is_exist_subcommand(struct lips_args* args, const char* name);
int lips_add_option(struct lips_args* args, const char* name);
int lips_add_option_long(struct lips_args* args, const char* name);
int lips_add_subcommand(struct lips_args* args, const char* name);
char* lips_get_option_value(struct lips_args* args, const char* name);
char* lips_get_option_long_value(struct lips_args* args, const char* name);
void lips_parse_args(struct lips_args* args, const int argc, char* argv[]);

struct lips_args* new_lips_args();
void delete_lips_args(struct lips_args* args);

#endif
