# minishell

readline			>	show promt and read line input
rl_clear_history	>
rl_on_new_line
rl_replace_line
rl_redisplay
add_history

printf
malloc
free
write
access
open
read
close
exit
strerror			>	(int errno) returns error msg for errno
perror				>	(char *str) print str and error msg

fork
wait
waitpid
wait3
wait4
execve
dup
dup2
pipe

signal				>	do not us (us sigaction)
sigaction
sigemptyset
sigaddset

kill
getcwd
chdir
stat
lstat
fstat
unlink

opendir
readdir
closedir

isatty
ttyname
ttyslot

ioctl
getenv

tcsetattr
tcgetattr
tgetent
tgetflag
tgetnum
tgetstr
tgoto
tputs





## SOURCES

> Documentation in README
> https://github.com/DimitriDaSilva/42_minishell?tab=readme-ov-file#1-extracting-information

### General Idea - Why write your own shell?
> https://www.cipht.net/2017/10/17/build-your-own-shell.html
> https://github.com/tokenrove/build-your-own-shell

> https://brennan.io/2015/01/16/write-a-shell-in-c/
> https://github.com/brenns10/lsh


### Parsing

Parsing without malloc (Andres)
> https://craftinginterpreters.com/scanning-on-demand.html

Matching wildcards
> https://www.geeksforgeeks.org/wildcard-pattern-matching/

Algorithms for parsing
> https://en.wikipedia.org/wiki/Shunting_yard_algorithm
> https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing
> https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf



## NOTES

Example for seperation of the tasks in this project:
    Extracting Information, Parsing, Termcaps, Executables, Pipes and Redirections
    Environment Variables, Builtin Functions and Signal Handling

