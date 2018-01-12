/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvisenti <tvisenti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 15:10:41 by tvisenti          #+#    #+#             */
/*   Updated: 2018/01/12 15:41:47 by tvisenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_p.h"

void	usage(char *str)
{
	ft_putstr("Usage: ");
	ft_putstr(str);
	ft_putendl(" <addr> <port>\n");
	exit(-1);
}

void	print_fd_err(char *str, int fd)
{
	ft_putendl(str);
	ft_putendl_fd(str, fd);
	write(fd, "\0", 1);
	return ;
}

void	print_fd(char *str, int fd)
{
	ft_putendl(str);
	ft_putendl_fd(str, fd);
}

int		print_error(char *str)
{
	ft_putstr("Error: ");
	ft_putendl(str);
	return (-1);
}