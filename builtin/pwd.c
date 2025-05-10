/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:35 by skock             #+#    #+#             */
/*   Updated: 2025/05/06 15:21:41 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
		ft_printf("%s\n", path);
	else
		perror("getcwd");
}
