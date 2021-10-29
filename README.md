# Ficus

## What is Ficus server?

The Ficus server or simply Ficus,
is a minimalist web server that aims to be lightweight and libre.
Ficus is built from the ground up without any external libraries
or dependencies and can be compiled with just a C compiler.
Ficus can run on unix or unix-like systems
such as Linux or a BSD derivative
(Windows and MacOS have not been tested and is out of scope for this project).
As long as you have a modern C compiler that targets the desired instruction set,
Ficus can be built for just about any platform.
Ficus is free and licensed under the GNU Public License Version 3.
View "COPYING" for more details

## How to build the Ficus server

The Ficus web server uses GNU Autotools for building, installation and distribution.

To install run:

```sh
./configure && make && make install
```

To uninstall run:

```sh
make uninstall
```
