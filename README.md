# Smallsh - A command line shell written in C
**Smallsh implements a subset of features of well-known shells such as bash or zsh.**

The syntax for commands is as follows:
```
command [arg1 arg2 ... [< input_file] [> output_file] [&]
```
... where items in square brackets are optional.

Smallsh supports three built-in commands (```exit```, ```cd```, and ```status```). Smallsh supports all other commands using '''fork()''' and '''exec()'''. The special symbols ```>```, ```<```, and ```&```` allow you to redirect I/O and run commands in the background. The shell also supports expansion of the variable `$$`, which is replaced by the process ID of smallsh itself.
