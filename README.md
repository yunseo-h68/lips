# Lips

Simple library for parsing command-line arguments.

## Build and Run

```
$ git clone https://github.com/yunseo-h68/lips.git

$ make example

$ ./bin/example
example | example of lips

Usage:
	example [OPTIONS] [SUBCOMMANDS]

Options:
	-h, --help : Display this information
	-v, --version : Display version
	    --echo=[STRING] : Display [STRING]

Subcommands:
	hello : print 'Hello'
	world : print 'World'

$ ./bin/example hello
Hello

$ ./bin/example --echo='hello world'
hello world

$ ./bin/example -v
version 0.1.3

```

## License

`Lips` is primarily distributed under the terms of MIT license.

See [LICENSE](./LICENSE) for details.
