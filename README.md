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



## SOURCES / ACKNOWLEDGEMENTS

> Documentation in README
> https://github.com/DimitriDaSilva/42_minishell?tab=readme-ov-file#1-extracting-information


### Shell
> https://www.cipht.net/2017/10/17/build-your-own-shell.html
> https://github.com/tokenrove/build-your-own-shell

> https://brennan.io/2015/01/16/write-a-shell-in-c/
> https://github.com/brenns10/lsh
> [Shell in C](https://www.youtube.com/watch?v=ubt-UjcQUYg)
> [Bash Architecture](https://aosabook.org/en/v1/bash.html)
> [Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)


### Parsing

Parsing without malloc (Andres)
> https://craftinginterpreters.com/scanning-on-demand.html

Matching wildcards
> https://www.geeksforgeeks.org/wildcard-pattern-matching/

Algorithms for parsing
> https://en.wikipedia.org/wiki/Shunting_yard_algorithm
> https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing
> https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf

> [Parsing Techniques A Practical Guide](https://freecomputerbooks.com/Parsing-Techniques-A-Practical-Guide.html)
> [History of Parsing methods](https://www.andrews.edu/~bidwell/456/history.html)

### Collaboration

[Github Guide on Collaboration](https://github.blog/2023-08-04-a-checklist-and-guide-to-get-your-repository-collaboration-ready/)
[Jonathan Mines Github Collaboration Guide](https://medium.com/@jonathanmines/the-ultimate-github-collaboration-guide-df816e98fb67)

