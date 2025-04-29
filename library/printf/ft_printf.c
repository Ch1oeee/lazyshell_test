/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:18:44 by skock             #+#    #+#             */
/*   Updated: 2024/12/02 13:11:47 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_p(unsigned long nb)
{
	int	len;

	if (!nb)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	len = ft_print_x(nb, "x");
	return (len + 2);
}

static int	ft_printf_arg(int i, int len, const char *format, va_list args)
{
	if (format[i] == 'd' || format[i] == 'i')
		len += ft_print_i_or_d(va_arg(args, int));
	else if (format[i] == 's')
		len += ft_print_s(va_arg(args, char *));
	else if (format[i] == 'c')
		len += ft_print_c((char)va_arg(args, int));
	else if (format[i] == 'x')
		len += ft_print_x(va_arg(args, unsigned int), "x");
	else if (format[i] == 'X')
		len += ft_print_x(va_arg(args, unsigned int), "X");
	else if (format[i] == '%')
		len += ft_print_c('%');
	else if (format[i] == 'p')
		len += ft_print_p(va_arg(args, unsigned long));
	else if (format[i] == 'u')
		len += ft_print_u(va_arg(args, unsigned int));
	return (len);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	int		i;
	va_list	args;

	len = 0;
	va_start(args, format);
	i = 0;
	while (format[i])
	{
		if (format[i] != '%')
		{
			ft_print_c(format[i]);
			len++;
		}
		else
		{
			i++;
			len = ft_printf_arg(i, len, format, args);
		}
		i++;
	}
	va_end(args);
	return (len);
}

// int	main(void)
// {
// 	int len;

// 	printf("Test avec printf :\n");
// 	len = printf("afficher %x", 9000);
// 	printf("len = %d\n", len);

// 	printf("Test avec ft_printf :\n");
// 	len = ft_printf("afficher %x", 9000);
// 	printf("len = %d\n", len);
// }
