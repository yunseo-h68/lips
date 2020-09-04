# Lips

Simple library for parsing command-line arguments.

Latest Version : v0.1.6

## Install and Build

1. Download the source code for this library using git or wget.

2. Build using Makefile.

3. Copy `lib` directory and `include/lips.h` to your project directory.

4. Create your own programs with this library.

```
# Using git
## For example, git -b v0.1.0 clone
$ git -b [LIPS VERSION] clone https://github.com/yunseo-h68/lips.git

# Or using wget
## For example, v0.1.0.zip
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

## License

`Lips` is primarily distributed under the terms of MIT license.

See [LICENSE](./LICENSE) for details.
