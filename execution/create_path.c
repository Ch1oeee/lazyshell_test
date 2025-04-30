/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 07:25:17 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/30 13:20:07 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
		
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

// POUR EXVEC
char	**tokens_to_args(t_token *token)
{
	int		count;
	t_token	*tmp;
	char	**args;
	int		i;
	
	i = 0;
	count = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD || tmp->type == EXPANDING || 
			tmp->type == S_QUOTE || tmp->type == D_QUOTE)
			count++;
		else if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT || 
				 tmp->type == APPEND || tmp->type == HEREDOC)
			tmp = tmp->next;
		if (!tmp) 
			break;
		tmp = tmp->next;
	}
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD || tmp->type == EXPANDING || 
			tmp->type == S_QUOTE || tmp->type == D_QUOTE)
		{
			args[i] = ft_strdup(tmp->value);
			i++;
		}
		else if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT || 
				 tmp->type == APPEND || tmp->type == HEREDOC)
		{
			if (tmp->next)
				tmp = tmp->next;
		}
		if (!tmp)
			break;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

char *get_env_value(t_env *env, char *key)
{
	t_env *current;
		
	current = env;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char *ft_strjoin3(char *s1, char *s2, char *s3)
{
	char *temp;
	char *result;
		
	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}