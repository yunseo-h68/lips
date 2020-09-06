# Lips

Simple library for parsing command-line arguments.

Latest Version : v1.0.0

- [About or Features](#about-or-features)
- [Install and Build](#install-and-build)
- [Usage](#usage)
  - [lips_args structure](#--lips_args-structure)
  - [lips_option structure](#--lips_option-structure)
  - [Creating and deleting the lips_args structure](#--creating-and-deleting-the-lips_args-structure)
  - [To add to the list of options and subcommands](#--to-add-to-the-list-of-options-and-subcommands)
  - [Argument parsing](#--argument-parsing)
  - [To check for specific options or subcommands](#--to-check-for-specific-options-or-subcommands)
  - [To get option values](#--to-get-option-values)
  - [To get other arguments](#--to-get-other-arguments)
  - [To get the number of options, subcommands, and other_args](#--to-get-the-number-of-options-subcommands-and-other_args)
- [Example](#example)
- [License](#license)



## About or Features

This library parses command-line arguments for command-line interface application. This library is very simple and easy to use. This library is created for my other projects. 

When you use this library, you should be the first to make a list of options, and subcommands. This library determines the type of arguments according to this list. After the library determines the type of arguments, you can check for specific options, subcommands to determine what action to take.

This library classifies arguments into four types.

- Subcommand
- Option(Short)
- Option(Long)
- Other Args

Arguments that do not begin with '-' are classified as `Subcommand` or `Other Args` type. If this argument is in the subcommand list, it is classified as a `Subcommand` type. The `Subcommand` type is granted to only one argument. The remaining arguments that do not start with '-' are all `Other Args` type.

`Option(Short)` starts with a single dash(`-`), such as '-h'. On the other hand, the `Option(Long)` type starts with two dashes(`--`). If the argument starts with more than three dashes, this type is `Other Args` type. If the argument is not in the option list, this is the `Other Args` type, not the `Option` type.

The `Option` type has a value. this library allows you to get the 'hello' value from '-t=hello' or '--test=hello'.



## Install and Build

1. Download the source code for this library using git or wget.

2. Build using Makefile.

3. Copy `lib` directory and `include/lips.h` to your project directory.

4. Create your own programs with this library.

```
# Using git
## For example, git -b v1.0.0 clone
$ git -b [LIPS VERSION] clone https://github.com/yunseo-h68/lips.git

# Or using wget
## For example, v1.0.0.zip
$ wget https://github.com/yunseo-h68/lips/archive/[LIPS VERSION].zip
$ unzip [LIPS VERSION].zip

$ cd [LIPS SRC DIR]
$ make
$ cp -r lib [PROJECT LIB DIR]
$ cp include/lips.h [PROJECT SRC DIR]/lips.h # lips.h

# Build with this library.
## For example, gcc main.c -L./lib -llips
$ gcc [.c src files] -L[PROJECT LIB DIR] -llips
```



## Usage

Functions with the return type `int` in this library return -1 if they fail.  If the return type is a pointer, return `NULL` if it fails.

### - lips_args structure

```c
struct lips_args {
	int count_options;
	int count_options_long;
	int count_options_short;
	int count_subcommands;
	int count_other_args;

	char* subcommand;
	char** other_args;

	struct lips_option** options;
	char** subcommands;
};
```

### - lips_option structure

```c
struct lips_option {
	int is_exist;
	int type;
	char* name;
	char* value;
};
```

### - Creating and deleting the lips_args structure

The `lips_args` structure is generally used in this library. It is also used to store processed results. To create these `lips_args` structures, use the function `new_lips_args()`. This function performs initialization on the `lips_args` structure and returns the pointer.

```c
struct lips_args* new_lips_args();
```

Use the `delete_lips_args()` function to free the memory of the dynamically allocated `lips_args` structure.

```c
void delete_lips_args(struct lips_args* args);
```

Example of using:

```c
struct lips_args* args = new_lips_args();
delete_lips_args(args);
```

### - To add to the list of options and subcommands

This library determines the type of arguments based on the list of options and subcommands. Therefore, options and subcommands should be added to the list. To add options to this list, use functions `lips_add_option()` and `lips_add_option_long()`.  If the type of option you want to add is `long`, use the function `lips_add_option_long()`.

```c
int lips_add_option(struct lips_args* args, const char* name);
int lips_add_option_long(struct lips_args* args, const char* name);
```

If you want to add to the subcommand list, use function `lips_add_subcommand()`.

```c
int lips_add_subcommand(struct lips_args* args, const char* name);
```

Example of using:

```c
// Add -h, --help Option
lips_add_option(args, "h");
lips_add_option_long(args, "help");

// Add hello Subcommand
lips_add_subcommand(args, "hello");
```

### - Argument parsing

If you have completed the list of options and subcommands, parse the arguments. This can be done through function `lips_parse_args()`.

```c
int lips_parse_args(struct lips_args* args, const int argc, char* argv[]);
```

Example of using:

```c
int main(int argc, char* argv[])
{
    struct lips_args* args = new_lips_args();
    add_options(args);
    add_subcommands(args);
    lips_parse_args(args, argc, argv);
    // ...
}
```

### - To check for specific options or subcommands

You can use these three functions below. These return 1 if what you want exists, or 0 if not.  For subcommands, you can also compare 'subcommand' fields.

```c
int lips_is_exist_option(struct lips_args* args, const char* name);
int lips_is_exist_option_long(struct lips_args* args, const char* name);
int lips_is_exist_subcommand(struct lips_args* args, const char* name);
```

Example of using:

```c
if (lips_is_exist_subcommand(args, "hello")) {
    // => args->subcommand != NULL && !strcmp(args->subcommand, "hello");
    printf("hello\n");
    delete_lips_args(args);
    return 0;
}

if (lips_is_exist_option(args, "h") || lips_is_exist_option_long(args, "help")) {
    print_help();
    delete_lips_args(args);
    return 0;
}

if (args->subcommand != NULL) {
	// Print Subcommand
	printf("Subcommand : %s\n", args->subcommand);
}
```

### - To get option values

Arguments of type `Option` may have values. And this library allows you to get this value through two functions, `lips_get_option_value()` and `lips_get_option_long_value()`.

```c
char* lips_get_option_value(struct lips_args* args, const char* name);
char* lips_get_option_long_value(struct lips_args* args, const char* name);
```

Example of using:

```c
void option_echo(struct lips_args* args)
{
    char* str = lips_get_option_long_value(args, "echo");
    if (str == NULL) {
        return;
    }
    printf("%s\n", str);
}
```

### - To get other arguments

If you want to get other arguments(not option or subcommand), use `lips_get_other()` or access `other_args` field.

```c
char** lips_get_other(struct lips_args* args);
args->other_args;
```

Example of using;

```c
void print_other(struct lips_args* args)
{
    int i = 0;
    // use lips_get_other()
    char** other = lips_get_other(args);
    for (i = 0; i < args->count_other_args; i++) {
        printf("%s\n", other[i]);
    }
    // or access other_args field
    for (i = 0; i < args->count_other_args; i++) {
        printf("%s\n", args->other_args[i]);
    }
}
```

### - To get the number of options, subcommands, and other_args

If you want to get the number of options, subcommands, and other_args, access the field that begins with 'count_' such as `count_options`,`count_options_long`,`count_options_short`, `count_subcommands`, `count_other_args`.

```c
struct lips_args {
	int count_options; // count_options_long + count_options_short
	int count_options_long;
	int count_options_short;
	int count_subcommands;
	int count_other_args;
    //...
};
```



## Example

```c
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
```



## License

`Lips` is primarily distributed under the terms of MIT license.

See [LICENSE](./LICENSE) for details.
