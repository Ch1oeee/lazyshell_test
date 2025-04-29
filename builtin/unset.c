/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:38:16 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/25 22:10:04 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_ms *ms, t_cmd *cmd)
{
	t_token	*arg;
	
	arg = cmd->token->next;

	while (arg)
	{
		if (arg->type == WORD && valid_name(arg->value))
		{
			ft_printf("unset worked\n");
			unset_var(ms, arg->value);
		}
		else if (arg->type == WORD)
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(arg->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ms->status = 1;
		}
		arg = arg->next;
	}
}

void	unset_var(t_ms *ms, char *key)
{
	t_env	*prev;
	t_env	*cur;

	prev = NULL;
	cur = ms->env_lst;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				ms->env_lst = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	valid_name(const char *str)
{
	int	i;
	
	if (!str || !*str)
		return (0);
	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

