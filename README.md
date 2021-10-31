# Ficus

## What is Ficus server?

The Ficus server or simply Ficus,
is a minimalist web server that aims to be lightweight and libre.
Ficus is built from the ground up with (almost) no external libraries
or dependencies and can be compiled with just a C compiler (and `libressl`).
Ficus can run on unix or unix-like systems
such as Linux or a BSD derivative
(Windows and MacOS have not been tested and is out of scope for this project).
Ficus is free and licensed under the GNU Public License Version 3.
View "COPYING" for more details

## How to build the Ficus server

The Ficus web server uses GNU Make for building, installation and distribution.

To install run:

```sh
sudo make install
```

To uninstall run:

```sh
sudo make uninstall
```

To make a tar-ball run:

```sh
make dist
```
