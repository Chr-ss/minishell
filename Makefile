NAME		=	minishell
NAME_DEBUG	=	minishell_debug
RM			=	rm -rf
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -Wunused -Wuninitialized -Wunreachable-code -g3 # -MMD -fsanitize=address # -Ofast

SRCDIR		=	src
SRC			=	$(shell find $(SRCDIR) -iname "*.c")

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
