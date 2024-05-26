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