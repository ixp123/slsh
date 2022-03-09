# slsh
slsh: Spicy Little SHell

Objectives:

[] cd with no arguments is aliased to the $USER home directory
[] extended prompt funtionality (i.e. hostname, cwd, etc. being displayed on the prompt)
[] CTRL+L and other bindings

# Why another shell?
The point of me making this shell, is to make a shell that fully adheres to the suckless philosophy and ideals

Another point being simplicity, this shell will remain simple, no complex "rc" or "profile" sourcing, EVERYTHING will be configured via config.h and recompilation

Final point being learning.

# Install

First, clone the repo:
```
git clone https://github.com/ixp123/slsh
```

Second, make
```
cd slsh
make
```

Third, install
```
doas make install
```
