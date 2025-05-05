

#include "../minishell.h"

void	join_expand_heredoc(t_ms *minishell, char **heredoc, int index)
{
	int			size;
	char		*word_expand;
	char		*tmp_str;
	t_token		*tmp;

	size = expand_size(minishell);
	tmp = minishell->expand;
	if (!tmp)
		return ;
	word_expand = ft_strdup(tmp->value);
	tmp = tmp->next;
	while (tmp && size-- >= 0)
	{
		tmp_str = ft_strjoin(word_expand, tmp->value);
		free(word_expand);
		word_expand = tmp_str;
		tmp = tmp->next;
	}
	free(heredoc[index]);
	heredoc[index] = ft_strdup(word_expand);
	free(word_expand);
}

void	expand_heredoc(t_ms *minishell)
{
	t_env	*tmp_env;
	t_token	*tmp2;

	tmp2 = minishell->expand;
	while (tmp2)
	{
		tmp_env = minishell->env_lst;
		while (tmp_env)
		{
			if (!ft_strcmp(tmp_env->key, tmp2->value + 1))
			{
				free(tmp2->value);
				tmp2->value = ft_strdup(tmp_env->value);
			}
			else if (tmp2->value[0] == '$'
				&& ft_strcmp(tmp_env->key, tmp2->value + 1) == 1)
			{
				free(tmp2->value);
				tmp2->value = ft_strdup("");
			}
			tmp_env = tmp_env->next;
		}
		tmp2 = tmp2->next;
	}
	return ;
}

void	word_expand_heredoc(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	while (value[*i] && value[*i] != '$')
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
}

void	dollar_expand_heredoc(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	(*i)++;
	while (ft_isalnum(value[*i]) || value[*i] == '_')
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
}

void	do_expand_heredoc(char **heredoc, t_ms *minishell)
{
	int	i;
	int	j;

	j = -1;
	while (heredoc[++j])
	{
		i = 0;
		while (heredoc[j][i])
		{
			if (heredoc[j][i] == '$')
				dollar_expand_heredoc(heredoc[j], minishell, &i);
			else
				word_expand_heredoc(heredoc[j], minishell, &i);
		}
		expand_heredoc(minishell);
		join_expand_heredoc(minishell, heredoc, j);
		free_expand_list(minishell);
	}
}
