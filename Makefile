NAME		=	minishell
NAME_DEBUG	=	minishell_debug
RM			=	rm -rf
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -Wunused -Wuninitialized -Wunreachable-code -g3 # -MMD -fsanitize=address # -Ofast

SRCDIR		=	src
SRC			=	src/token/type_handler_redirect.c	\
				src/token/openfile.c	\
				src/token/printf_cmd.c	\
				src/token/token.c	\
				src/token/cmd.c	\
				src/token/type_handler.c	\
				src/init/initdata.c	\
				src/input_handling.c	\
				src/utils/cmd_reset.c	\
				src/utils/memory.c	\
				src/utils/etc.c	\
				src/utils/io.c	\
				src/utils/debug.c	\
				src/utils/utils.c	\
				src/utils/envp.c	\
				src/utils/add_envp.c	\
				src/utils/get_envp.c	\
				src/heredoc/heredoc.c	\
				src/builtins/unset/unset.c	\
				src/builtins/echo/echo_getline.c	\
				src/builtins/echo/echo.c	\
				src/builtins/pwd/pwd.c	\
				src/builtins/env/env.c	\
				src/builtins/exit/exit.c	\
				src/builtins/cd/cd_parse.c	\
				src/builtins/cd/cd_oldpwd.c	\
				src/builtins/cd/cd.c	\
				src/builtins/cd/cd_parse_check.c	\
				src/builtins/export/export_envp.c	\
				src/builtins/export/export.c	\
				src/builtins/export/export_print.c	\
				src/expansion/expansion_utils.c	\
				src/expansion/expansion.c	\
				src/childs/init_kindergarten.c	\
				src/childs/manipulate_child.c	\
				src/childs/terminate_children.c	\
				src/main.c	\
				src/execution/execution_exit.c	\
				src/execution/execution_argv.c	\
				src/execution/execution_child_dup.c	\
				src/execution/execution_wait.c	\
				src/execution/execution_child.c	\
				src/execution/execution.c	\
				src/execution/execution_child_dup_fd.c	\
				src/execution/execution_path.c	\
				src/parsing/parsing.c	\
				src/error/error.c	\
				src/signal/signal_handler.c	\
				src/signal/signal.c

OBJDIR		=	build
OBJ			=	$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBFT		=	libft/libft.a
SUBMOD_FLAG	=	libft/Makefile


all:		$(NAME)

debug: CFLAGS += -DDEBUG
debug: fclean $(NAME)

test: CFLAGS += -DTEST
test: fclean $(NAME)

$(NAME):	$(SUBMOD_FLAG) $(LIBFT)	$(OBJ)
			@$(CC) $(OBJ) $(LIBFT) -o $(NAME) -lreadline
			@printf "$(CREATED)" $@ $(CUR_DIR)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@
			@printf "$(UPDATED)" $@ $(CUR_DIR)

$(SUBMOD_FLAG):
			git	submodule	init
			git	submodule	update

$(LIBFT):
			@$(MAKE) --no-print-directory -C $(@D) all
			@printf "$(MADE)" $@ $(dir $(abspath $(LIBFT)))

clean:
			$(RM) $(OBJDIR)
			@printf "$(REMOVED)" $(OBJDIR) $(CUR_DIR)

fclean: clean
			$(RM) $(NAME) $(NAME_DEBUG)
			@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:			fclean all

info-%:
	$(info $($*))

.PHONY: all clean fclean re
.SILENT:

# ----------------------------------- colors --------------------------------- #

BOLD	:= \033[1m
BLACK	:= \033[30;1m
RED		:= \033[31;1m
GREEN	:= \033[32;1m
YELLOW	:= \033[33;1m
BLUE	:= \033[34;1m
MAGENTA	:= \033[35;1m
CYAN	:= \033[36;1m
WHITE	:= \033[37;1m
RESET	:= \033[0m

# ----------------------------------- messages ------------------------------- #

CUR_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED	:= \t$(RED)$(BOLD)REMOVED %s (%s) $(RESET)\n
MADE	:= \t$(GREEN)$(BOLD)MAKE -C %s (%s) $(RESET)\n
CREATED	:= \t$(GREEN)$(BOLD)CREATED %s (%s) $(RESET)\n
UPDATED	:= \t$(BLUE)$(BOLD)CREATED OR UPDATED %s (%s) $(RESET)\n
