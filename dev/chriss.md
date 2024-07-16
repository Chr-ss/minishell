# Chriss NOTES #
info for sebastian, birthday boy

redirection on one command not working
export first character cannot be number
export abc -> not in order, and maybe not = after abc
and if you export somehting without value, it should not be printed when you do env

./minishell
./minishell
everytime shell level in env >> SHLVL +1, and exit SHLVL -1
after cd, your envp PWD become relative path, not absolute
unset is not unsetting


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