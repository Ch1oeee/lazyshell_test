/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:43 by skock             #+#    #+#             */
/*   Updated: 2025/04/25 22:56:16 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*lst_new(t_ms *ms)
{
	t_env		*env;
	static int	i = 0;

	env = malloc(sizeof(t_env));
	env->index = i + 1;
	env->key = cpy_left(ms->envp[i]);
	env->value = cpy_right(ms->envp[i]);
	env->next = NULL;
	i++;
	return (env);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	fill_env_cpy(t_ms *ms, char **envp)
{
	t_env	*env;
	int		i;

	ms->env_lst = NULL;
	i = 0;
	while (envp[i])
	{
		env = lst_new(ms);
		if (!env)
			return ;
		ft_lstadd_back(&ms->env_lst, env);
		i++;
	}
}

char	*cpy_left(char	*env_var)
{
	int		i;
	char	*key;

	i = 0;
	while (env_var[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i)+ 1);
	if (!key)
		exit(1);
	i = 0;
	while (env_var[i] != '=')
	{
		key[i] = env_var[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*cpy_right(char *env_var)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	while (env_var[i] != '=')
		i++;
	j = ++i;
	while (env_var[i])
		i++;
	value = malloc(sizeof(char) * (i - j + 1));
	if (!value)
		exit(1);
	i = 0;
	while (env_var[j])
	{
		value[i] = env_var[j];
		i++;
		j++;
	}
	value[i] = '\0';
	return (value);
}
