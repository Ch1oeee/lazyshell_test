/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:21:24 by skock             #+#    #+#             */
/*   Updated: 2025/04/25 22:00:46 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <stdio.h>
# include <stdarg.h>
# include <limits.h>
# include <fcntl.h>
# include "../printf/ft_printf.h"
# include "../gnl/get_next_line.h"

int		ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(char c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_iswhitespace(int c);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s);
char	*ft_itoa(int n);
char	*ft_strjoin_slash(char *s1, char const *s2);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_bzero(void *s, size_t n);
size_t	ft_strlen(const char *str);
int		ft_strnlen(const char *str, char cut);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strstr(const char *haystack, const char *needle);
void	ft_strclear(char *str, const char *remove);
char	*ft_strrchr(const char *str, int c);
char	*ft_strchr_lib(const char *str, int c);

#endif
