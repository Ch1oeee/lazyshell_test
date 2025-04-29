/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:59:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/13 10:25:05 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_ms *minishell)
{
	t_env	*current;
	t_env	*old;

	old = minishell->env_lst;
	current = minishell->env_lst;
	while (current)
	{
		if (!ft_strcmp(current->key, "PWD"))
			break ;
		current = current->next;
	}
	while (old)
	{
		if (!ft_strcmp(old->key, "OLDPWD"))
		{
			free(old->value);
			old->value = current->value;
		}
		old = old->next;
	}
	current->value = getcwd(NULL, 0);
}

char	*get_last_folder(char *path)
{
	int		i;

	i = 0;
	while (path[i])
		i++;
	while (path[i] != '/')
	{
		path[i] = '\0';
		i--;
	}
	return (path);
}

char	*get_user(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "HOME"))
		{
			break ;
		}
		tmp = tmp->next;
	}
	return (tmp->value);
}

char	*get_oldpwd(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "OLDPWD"))
			break ;
		tmp = tmp->next;
	}
	return (tmp->value);
}
