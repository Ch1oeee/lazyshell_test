/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:02:35 by skock             #+#    #+#             */
/*   Updated: 2024/12/17 12:18:11 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_c(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_print_s(char *str)
{
	int				count;
	unsigned int	i;

	i = 0;
	count = 0;
	if (!str)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	while (str[i])
	{
		count += ft_print_c(str[i]);
		i++;
	}
	return (count);
}

int	ft_print_x(unsigned long nb, char *format)
{
	int	count;

	count = 0;
	if (nb >= 16)
	{
		count += ft_print_x(nb / 16, format);
		count += ft_print_x(nb % 16, format);
	}
	else
	{
		if (*format == 'X')
			count += ft_print_c("0123456789ABCDEF"[nb % 16]);
		else
			count += ft_print_c("0123456789abcdef"[nb % 16]);
	}
	return (count);
}

int	ft_print_u(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_print_u(nb / 10);
		ft_print_u(nb % 10);
	}
	if (nb <= 9)
		ft_print_c('0' + nb);
	count ++;
	return (count);
}

int	ft_print_i_or_d(int nb)
{
	char	*str;
	int		i;

	str = ft_itoa(nb);
	i = 0;
	while (str[i])
	{
		ft_print_c(str[i]);
		i++;
	}
	free(str);
	return (i);
}
