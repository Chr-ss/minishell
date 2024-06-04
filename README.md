# minishell

This project is about creating a Bourne-Again Shell (BASH) replica for the minishell project at Codam.
Minishell is therefore a user-mode program that implements a Command Line Interface (CLI).

In general terms what this program does is:

- Print a prompt
- Read a line
- Parse
- Execute
- Repeat

**Features**

_options_
```sh
minishell:~$ pgrm -r -f path/to/file
```

_i/o redirections_
```sh
minishell:~$ pgrm < infile > outfile
minishell:~$ pgrm < infile >> outfile
```
_pipes_
```sh
minishell:~$ pgrm | pgrm2 | pgrm3
```

_enviroment variables_
```sh
minishell:~$ARG=something
minishell:~$ pgrm $ARG
```

_previous foreground exit status_
```sh
minishell:~$ pgrm $?
```

_single quotes_
```sh
minishell:~$ pgrm '$ARG $ARG2'
```

_double quotes_
```sh
minishell:~$ pgrm "$ARG $ARG2"
```

_key bindings_
- arrow up / down for history.
- ctrl-C displays a new prompt on a new line.
- ctrl-D exits the shell.
- ctrl-\ does nothing.

**Built-ins**

- echo with option -n
- cd with only a relative or absolute path
- pwd with no options
- export with no options
- unset with no options
- env with no options or arguments
- exit with no options

**Missing**

- scripting control
- command / filename completion
- background control
- initialization / startup files

**system calls**

- fork
- exec
- exit
- wait
- open
- close
- read
- write
- pipe
- dup
- chdir


## GETTING STARTED

1. Clone the repo


```sh
git clone https://github.com/Chr-ss/minishell.git
```
2. cd into directory

```sh
cd minishell
```

3. Call the Makefile

```sh
make
```


## Other Docs
In addition to the README you're reading right now, there are other documents which describe more about this project:

- [docs/ARCHITECTURE.md](https://github.com/Chr-ss/minishell/tree/main/docs/ARCHITECTURE.md)
- [docs/CONTRIBUTING.md](https://github.com/Chr-ss/minishell/tree/main/docs/CONTRIBUTING.md)
- [docs/CODEOWNERS](https://github.com/Chr-ss/minishell/tree/main/docs/CODEOWNERS)
- [docs/SOURCES.md](https://github.com/Chr-ss/minishell/tree/main/docs/SOURCES.md)
