/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:44:02 by skock             #+#    #+#             */
/*   Updated: 2025/04/30 13:36:04 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <unistd.h>
# include <stdbool.h>
# include "library/libft/libft.h"

typedef enum e_type
{
	WORD = 1,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	S_QUOTE,
	D_QUOTE,
	EXPANDING,
}	t_type;

typedef struct s_token
{
	char			*value;
	bool			is_next_space;
	t_type			type;
	int				index;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	char			*path;
	int				infile_fd;
	int				outfile_fd;
	bool			is_pipe; //
	bool			is_redir; //
	pid_t			pid;
	struct s_token	*token;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_ms
{
	const char	*prompt_msg;
	int			status;
	char		**envp;
	bool		is_next_space;
	t_env		*env_lst;
	t_token		*token;
	t_token		*expand;
	t_cmd		*cmd_list;
	int			pipe_fd[2]; //
}				t_ms;

///////////////// PARSING /////////////////

// ENV(LIST && UTILS)

t_env	*lst_new(t_ms *ms);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*cpy_left(char	*env_var);
char	*cpy_right(char *env_var);
void	fill_env_cpy(t_ms *ms, char **envp);

// PROMPT (see later)

char	*return_last_folder(const char *path);
char	*get_last_path(t_ms *minishell);
void	get_input_prompt(t_ms *minishell);

// TOKENIZER (+LIST))

t_token	*new_token(char *str, t_ms *minishell, t_type type);
int		double_quote(char *input, int *i, t_ms *minishell);
int		single_quote(char *input, int *i, t_ms *minishell);
int		parsing_input(char *input, t_ms *minishell);
int		parsing_input(char *input, t_ms *minishell);
int		process_token(char *input, int *i, t_ms *minishell);
void	word_token(char *input, int *i, t_ms *minishell);
void	token_add_back(t_token **lst, t_token *new);
void	special_token(char *input, int *i, t_ms *minishell);
void	fill_token_list(t_ms *minishell, char *str, t_type type);

// MERGER

void	merge_token(t_ms *minishell);
void	merge_inception(t_ms *minishell);

// EXPANDER

void	expand_token(t_token *token, t_ms *minishell);
void	do_expand(char *value, t_ms *minishell, int index);
void	modify_main_token_lst(t_ms *minishell, char *word, int index);
int		expand_size(t_ms *minishell);
void	dollar_expand(char *value, t_ms *minishell, int *i);
void	word_expand(char *value, t_ms *minishell, int *i);
void	fill_expand_lst(t_ms *minishell, char *str);
void	expand_add_back(t_token **lst, t_token *new);
t_token	*new_expand(char *str);
void	join_expand(t_ms *minishell, int index);
void	expand(t_ms *minishell);

// HEREDOC EXPANDER

void	do_expand_heredoc(char **value, t_ms *minishell);
void	dollar_expand_heredoc(char *value, t_ms *minishell, int *i);
void	word_expand_heredoc(char *value, t_ms *minishell, int *i);
void	expand_heredoc(t_ms *minishell);
void	join_expand_heredoc(t_ms *minishell, char **heredoc, int index);


// CLEAR

char	*quote_rmv(const char *str);
void	clear_quote(t_ms *minishell);

// CMD_LST

void	fill_cmd_lst(t_ms *minishell);
void	cut_weird(t_cmd *cmd);
t_cmd	*create_cmd_until_pipe(t_token *tmp);
t_cmd	*new_cmd(void);

///////////////// EXECUTION /////////////////

char	**tokens_to_args(t_token *token);
int		execute_pipeline(t_ms *minishell);
int		process_redirections(t_ms *minishell);
char	*find_command_path(char *cmd, t_env *env);
bool	is_builtin(char *cmd);
char	*get_env_value(t_env *env, char *key);
char	*ft_strjoin3(char *s1, char *s2, char *s3);
void	free_array(char **array);
void	reset_commands(t_ms *minishell);
int		run_builtin_command(t_ms *minishell, t_cmd *temp_cmd, char **args);
int		create_token_chain(t_token *first_token, char **args);
int		setup_pipes(t_cmd *cmd, int *pipe_fd, int *prev_pipe);
int		execute_cmd(t_ms *ms, t_cmd *cmd, char **args, int *pipe_fd, int prev, int *status);
void	update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe);
int		wait_all_children(t_ms *ms, int last_pid, int last_status);

///////////////// BUILTIN /////////////////

int execute_builtin(t_ms *minishell, char **args);

// CD

void	cd(t_cmd *cmd, t_ms *ms);
void	update_pwd(t_ms *minishell);
void	go_tilde(t_ms *minishell);
void	go_back(t_ms *minishell);
void	go_root(t_ms *minishell);
void	go_old(t_ms *minishell);
char	*get_oldpwd(t_ms *minishell);
char	*get_last_folder(char *path);
char	*get_user(t_ms *minishell);
char	*get_last_dir(char *path);

// ENV

void	print_env(t_ms *minishell);

// PWD

void	print_pwd(void);

// ECHO

void	print_echo(t_cmd *cmd);

// EXIT

void	ft_exit(t_cmd *cmd, t_ms *minishell);

// EXPORT

void	ft_export(t_ms *ms, t_cmd *cmd);
void	export_set_var(t_ms *ms, char *arg);
int		env_update_or_add(t_env **env_lst, char *key, char *value);
void	update_envp(t_ms *ms);
void	export_print_sorted(t_env *env_lst);
char	*concat_env_var(char *key, char *value);

// UNSET

int		valid_name(const char *str);
void	ft_unset(t_ms *ms, t_cmd *cmd);
void	unset_var(t_ms *ms, char *key);

///////////////// PRINT /////////////////

void	print_cmd(t_cmd *cmd);
void	print_tokens(t_token *tokens);
void	print_expand(t_ms *minishell);

///////////////// FREE /////////////////

void	free_expand_list(t_ms *minishell);
void	free_env(t_ms *minishell); //
void	free_token_list(t_token *token);
void	free_cmd_list(t_cmd *cmd);
void	free_minishell(t_ms *minishell);

#endif
