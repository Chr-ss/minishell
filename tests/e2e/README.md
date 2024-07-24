test minishell against bash with all cases

watch out that minishell does not have tty test which exit when stdin, stdout, and stderror are not tty
because then the tests will not work. it is recommended to comment this function out to run tester
