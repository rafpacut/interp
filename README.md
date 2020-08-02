µC language interpreter.
=======================

A staticly typed, imperative, C-like language implementing integer type, flow control statements, dynamic arrays, functions, scope and recursion mechanisms, statement-by-statement debug mode and couple arbitrary features.

Written in c++ using boost Spirit X3 library for parsing and error reporting.

Basic documentation available [here](https://rafpacut.github.io/uC/).

How to build:
-------------
Download [boost library](https://www.boost.org/users/history/version_1_69_0.html) (tested on 1_69_0) and link to it changing ROOT_BOOST variable in Makefile.
```bash
git clone git@github.com:rafpacut/uC.git
cd uC/
make build
```
