# Lips

Simple library for parsing command-line arguments.

## Build and Run

```
$ git clone https://github.com/yunseo-h68/lips.git
$ make example
$ ./bin/example --help
example | example of lips

Usage:
	example [OPTIONS] [SUBCOMMANDS]

Options:
	-h, --help : Display this information
	-v, --version : Display version

Subcommands:
	hello : print 'Hello'
	world : print 'World'

$ ./bin/example hello
Hello
$ ./bin/example -v
version 0.1.0
```

## License

`Lips` is primarily distributed under the terms of MIT license.

See [LICENSE](./LICENSE) for details.
