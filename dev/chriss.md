# Chriss NOTES #


### readline ignore leaks?? ##

<https://github.com/simon-zerisenay/42_Minishell/blob/main/check_leaks.sh>



## unexpected_token handling ##

crasche@f1r6s19:~/github/minishell$ cat <<<<<< src/main.c | ls
bash: syntax error near unexpected token `<<<'
crasche@f1r6s19:~/github/minishell$ cat << < src/main.c | ls
bash: syntax error near unexpected token `<'
crasche@f1r6s19:~/github/minishell$ cat << < | ls
bash: syntax error near unexpected token `<'

temp = data->cmd_head;
while (temp)
{
	temp->infd = -1;
	temp = temp->pipe;
}
does this work???