# Smallsh - A command line shell written in C

Smallsh is a shell that implements a subset of features of well-known shells such as bash or zsh. The syntax for a command is as follows:
```
command [arg1 arg2 ... [< input_file] [> output_file] [&]
```
where items in square brackets are optional. Any command beginning with ```#``` will be treated as a comment and ignored. Smallsh commands support expansion of the variable ```$$``` into the PID of the shell.

If standard input or output is to be redirected, ```>``` or ```<``` followed by a filename word must appear after all the arguments. Input redirection can appear before or after output redirection.

If the command is to be executed in the background, the last word must be ```&```. If the ```&``` character appears anywhere else, it will be treated as normal text.

Smallsh supports three built-in commands:
* ```exit```
* ``cd```
* ```status```

All other commands are implemented using using '''fork()''' and '''exec()'''.
