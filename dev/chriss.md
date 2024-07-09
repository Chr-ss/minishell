# Chriss NOTES #

TO-DO
EXPANSION: $?
HEREDOC: NOT WORKING
FILEOPENING: IN AND OUT REDIRECTION

























At some point we have to take care of expanding still (most likely at the very beginning of the code...)
 $PATH

check string for syntax errors
 crasche@f0r2s6:~$ | ls | ls
 bash: syntax error near unexpected token `|'
we don't meed to handle open quotes
 crasche@f0r2s6:~$ echo "hello
 > ^C
tycho said not make this spacial case
 crasche@f0r2s6:~$ echo "hello;\hello"
 hello;\hello
 crasche@f0r2s6:~$ echo "hello";\"hello"
 > ^C
 crasche@f0r2s6:~$ echo "hello"a b"hello"
 helloa bhello
 crasche@f0r2s6:~$ echo "hello"ab"hello"
 helloabhello
handle this too
 crasche@f0r2s6:~$ ls | ls <
 bash: syntax error near unexpected token `newline'

for bonus we'd need this
 && ||
this we don't need (would make strings hard according to tycho)
 ; \

analyze token
 token 1 and token 2 (token )

## RIK NOTES ##

typedef void    (*t_builtin)(t_buffer*, t_lexer*);

rverhoev@f0r6s12:~/CORE/MiniRT$ export v="heelooo  $v$v okee nicee "
rverhoev@f0r6s12:~/CORE/MiniRT$ echo $v
heelooo heelooo okee nicee heelooo okee nicee okee nicee
rverhoev@f0r6s12:~/CORE/MiniRT$ export v="ls -l"
New

expand enviroment variable of multiple words into the to be tokenized string BEFOre the tokenizer :upside_down_face:

## EXIT AND CMD + D ##

ctrl-D exits the shell.
exit with no options

prints exit ind strerr!!

## EXPANDING NOTES ##

start with
$abcxyz
$ABCXYZ
$?
$_

### readline ignore leaks?? ##

<https://github.com/simon-zerisenay/42_Minishell/blob/main/check_leaks.sh>

readline   > show promt and read line input
rl_clear_history >
rl_on_line
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
strerror   > (int errno) returns error msg for errno
perror    > (char *str) print str and error msg

fork
wait
waitpid
wait3
wait4
execve
dup
dup2
pipe

signal    > do not us (us sigaction)
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

## TEST ##


$() does substitution
for file in $(ls)
do
 echo "Processing file: $file"
done

${} is for expansion/manipulation
name="John Doe"
echo ${name:0:4}
*this is not necessary

(  list of commands  )
this starts a subshell to excecute commands
env will not be affected and such

{ list of commands; }
in current shell
*this is not necessary

about precedence () in && and || logic :
https://velog.io/@soongle/cmd-priority-with-logical-operator-and-parenthesis

bash> echo a || (echo b && echo c) && echo d
a
d
bash> echo a || (echo b && (echo c && echo d))
a


bash> echo "1" && echo "2" || echo "3"
1
2
groups(1 AND 2) || 3

bash> echo "1" || echo "2" && echo "3"
1
3
groups(1 OR 2) AND 3


