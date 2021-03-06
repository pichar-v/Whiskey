# Whiskey

[![Build Status](https://travis-ci.org/w3st3ry/Whiskey.svg?branch=master)](https://travis-ci.org/w3st3ry/Whiskey)

**Whiskey** lets you write server-side web applications easily.
It's a new generation language embedded web framework and
lot of tools to simplify your programmer life.


## Dependencies

To build Whiskey, you need:

- A linux-based system — Currently, Windows and Mac OS X are not supported

- A C99 compiler — Clang and GCC are supported

- [SCons] — It’s a powerful `make` alternative

- Eventually the GNU Readline Library, without it the REPL is annoying.


## Building Whiskey

Just type `scons` in a terminal to run SCons:

```
$ scons
```

You can also use `make`, it runs `scons` too.


## Testing Whiskey

Once everything is built, you can run the `test` binary in the `test/`
directory. You can use Valgrind with the given suppressions file:

```
$ valgrind --suppressions=valgrind.supp ./test/test
```


## :rocket: Help us

Fork with us!

[SCons]: http://www.scons.org/
