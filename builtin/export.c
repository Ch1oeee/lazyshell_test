/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:06:33 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/12 16:17:42 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_set_var(t_ms *ms, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	equal_sign = ft_strchr_lib(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (!key || (!value && equal_sign[1] != '\0'))
		{
			free(key);
			free(value);
			ms->status = 1;
			return ;
		}
	}
	else
		key = ft_strdup(arg);
	if (!key)
	{
		ms->status = 1;
		return ;
	}
	if (env_update_or_add(&ms->env_lst, key, value) != 0)
		ms->status = 1;
	free(key);
	free(value);
}

int	env_update_or_add(t_env **env_lst, char *key, char *value)
{
	t_env	*current;
	t_ms	*ms = NULL;

	current = *env_lst;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value != NULL)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return (0);
		}
		current = current->next;
	}
	current = malloc(sizeof(t_env));
	if (current == NULL)
		return (ms->status = 1, 1);
		
	current->key = ft_strdup(key);
	if (value != NULL)
		current->value = ft_strdup(value);
	else
		current->value = NULL;
	current->index = 0;
	current->next = *env_lst;
	*env_lst = current;
	return (0);
}

void	update_envp(t_ms *ms)
{
	t_env	*env;
	int		count;
	int		i;

	env = ms->env_lst;
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	ms->envp = malloc(sizeof(char *) * (count + 1));
	if (!ms->envp)
		return;
	env = ms->env_lst;
	i = 0;
	while (env)
	{
		ms->envp[i] = concat_env_var(env->key, env->value);
		i++;
		env = env->next;
	}
	ms->envp[i] = NULL;
}

void	export_print_sorted(t_env *env_lst)
{
	t_env	**array;
	t_env	*env;
	int		count;
	int		i;
	int		swapped;

	env = env_lst;
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	array = malloc(sizeof(t_env *) * (count + 1));
	if (!array)
		return ;
	env = env_lst;
	i = 0;
	while (env)
	{
		array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (array[i + 1])
		{
			if (ft_strcmp(array[i]->key, array[i + 1]->key) > 0)
			{
				env = array[i];
				array[i] = array[i + 1];
				array[i + 1] = env;
				swapped = 1;
			}
			i++;
		}
	}
	i = 0;
	while (array[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(array[i]->key, STDOUT_FILENO);
		if (array[i]->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(array[i]->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	free(array);
}

char	*concat_env_var(char *key, char *value)
{
	char	*result;
	char	*temp;

	if (!value)
		return ft_strdup(key);

	result = ft_strdup(key);
	if (!result)
		return NULL;

	temp = ft_strjoin(result, "=");
	free(result);

	if (!temp)
		return NULL;

	result = ft_strjoin(temp, value);
	free(temp);

	return result;
}

int	ft_export(t_ms *ms, t_cmd *cmd)
{
	t_token	*arg;
	char	*equal_sign;
	char	*key;

	arg = cmd->token->next;
	ms->status = 0;
	if (!arg)
	{
		export_print_sorted(ms->env_lst);
		return (ms->status = 1, 1);
	}
	while (arg)
	{
		if (arg->type == WORD)
		{
			equal_sign = ft_strchr_lib(arg->value, '=');
			if (equal_sign)
				key = ft_substr(arg->value, 0, equal_sign - arg->value);
			else
				key = ft_strdup(arg->value);
			if (!valid_name(key))
			{
				ft_putstr_fd("export: `", STDERR_FILENO);
				ft_putstr_fd(arg->value, STDERR_FILENO);
				ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
				free(key);
				return (ms->status = 1, 1);
			}
			else
			{
				export_set_var(ms, arg->value);
				free(key);
			}
		}
		arg = arg->next;
	}
	update_envp(ms);
	return (ms->status);
}
