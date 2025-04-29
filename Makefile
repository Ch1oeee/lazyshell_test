NAME = minishell
CC = cc
FLAGS = -Wall -Werror -Wextra #-g -fsanitize=address
RM = rm -rf
LIBFT = library/libft/libft.a
LIBFT_FLAGS = -L./library/libft -lft
RDL_FLAG = -lreadline

SRCS	=	main.c\
			print.c\
			parsing/merge.c\
			parsing/cmd_lst.c\
			parsing/prompt.c\
			parsing/env.c\
			parsing/tokenizer.c\
			parsing/tokenizer_lst.c\
			parsing/quote_remove.c\
			parsing/process_token.c\
			parsing/expander.c\
			parsing/expander_end.c\
			parsing/expander_lst.c\
			parsing/free_expand.c\
			execution/exec.c\
			execution/free.c\
			execution/create_path.c\
			execution/exec_utils.c\
			execution/exec_redirection.c\
			builtin/env.c\
			builtin/pwd.c\
			builtin/cd.c\
			builtin/cd_utils.c\
			builtin/echo.c\
			builtin/exit.c\
			builtin/export.c\
			builtin/unset.c\


RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix obj/, $(OBJS))
OBJ_DIRS = $(sort $(dir $(OBJS)))

TARGET = $(NAME)

define PRINT_LOADING
	@echo -n "$(GREEN)Compiling... [                    ] 0%%$(RESET)\r"
	@for i in $(shell seq 1 20); do \
		sleep 0.035; \
		printf "\r$(GREEN)Compiling... ["; \
		printf "%*s" $$i | tr ' ' '#'; \
		printf "%*s" $$((20-$$i)) | tr ' ' ' '; \
		printf "] $$(($$i*5))%%$(RESET)"; \
	done
	@echo "\r$(GREEN)Compiling... [####################] 100%%$(RESET)"
	@clear
endef

$(NAME): $(LIBFT) $(OBJ_DIRS) $(OBJS)
	@$(PRINT_LOADING)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT_FLAGS) $(RDL_FLAG)
	@echo "$(CYAN)██╗      █████╗ ███████╗██╗   ██╗$(RESET)███████╗██╗  ██╗███████╗██╗     ██╗"
	@echo "$(CYAN)██║     ██╔══██╗╚══███╔╝╚██╗ ██╔╝$(RESET)██╔════╝██║  ██║██╔════╝██║     ██║"
	@echo "$(CYAN)██║     ███████║  ███╔╝  ╚████╔╝ $(RESET)███████╗███████║█████╗  ██║     ██║"
	@echo "$(CYAN)██║     ██╔══██║ ███╔╝    ╚██╔╝  $(RESET)╚════██║██╔══██║██╔══╝  ██║     ██║"
	@echo "$(CYAN)███████╗██║  ██║███████╗   ██║   $(RESET)███████║██║  ██║███████╗███████╗███████╗"
	@echo "$(CYAN)╚══════╝╚═╝  ╚═╝╚══════╝   ╚═╝   $(RESET)╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"

$(LIBFT):
	@make -s -C library/libft

$(OBJ_DIRS):
	@mkdir -p $@

obj/%.o: %.c | $(OBJ_DIRS)
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clear:
	@$(RM) $(OBJS)

clean: clear
	@make -s -C library/libft clean

fclean: clean
	@$(RM) $(NAME) obj/
	@make -s -C library/libft fclean

re: fclean all

.PHONY: all clean fclean re clear
