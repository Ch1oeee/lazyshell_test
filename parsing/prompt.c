/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:55:20 by skock             #+#    #+#             */
/*   Updated: 2025/03/27 13:32:41 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*return_last_folder(const char *path)
{
	int		i;
	int		j;
	char	*current_path;

	current_path = NULL;
	i = 0;
	while (path[i])
		i++;
	j = i;
	while (path[i] != '/')
		i--;
	i++;
	current_path = malloc(sizeof(char) * (j - i + 1));
	if (!current_path)
		exit(1);
	j = 0;
	while (path[i])
		current_path[j++] = path[i++];
	current_path[j] = '\0';
	return (current_path);
}

char	*get_last_path(t_ms *minishell)
{
	char	*current_path;
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PWD"))
		{
			current_path = return_last_folder(tmp->value);
			return (current_path);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	get_input_prompt(t_ms *minishell)
{
	char	*base;
	char	*base_2;

	base = "\033[0;36mlazy\033[0mshell:[";
	base_2 = "] ➜ $";
	minishell->prompt_msg = ft_strjoin(base, get_last_path(minishell));
	minishell->prompt_msg = ft_strjoin(minishell->prompt_msg, base_2);
}
