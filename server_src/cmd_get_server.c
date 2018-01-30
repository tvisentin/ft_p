/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_get_server.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvisenti <tvisenti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 09:43:50 by tvisenti          #+#    #+#             */
/*   Updated: 2018/01/30 17:26:42 by tvisenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_p.h"

static int		open_file_get_server(char *cmd, int client)
{
	char		*filename;
	int			file;

	filename = cmd;
	while (*filename == ' ')
		++filename;
	if ((file = open(filename, O_RDONLY)) == -1)
		ft_putendl_fd("ERROR_FD", client);
	else
		ft_putendl_fd("SERVER_OK", client);
	return (file);
}

static void		send_get_server(int fd, struct stat st, void *ptr)
{
	char		*size;

	size = ft_itoa(st.st_size);
	ft_putendl_fd(size, fd);
	free(size);
	if (recv_alert("SEND", fd) < 1)
		return (print_error_get_put("can't get size from client side"));
	send(fd, ptr, st.st_size, 0);
	munmap(ptr, st.st_size);
	if (recv_alert("SUCCESS", fd) == 1)
		print_fd("\033[32mSUCCESS: get\033[0m", fd);
}

void			cmd_get_server(int fd, char *buf)
{
	int			file;
	struct stat	st;
	void		*ptr;

	if ((file = open_file_get_server(buf, fd)) == -1)
		return (print_error_get_put("open() server side failed"));
	if (recv_alert("VERIF_FD", fd) < 1)
		return (print_error_get_put("open() client side failed"));
	if ((fstat(file, &st)) == -1)
	{
		ft_putendl_fd("TEST_ERROR", fd);
		return (print_error_get_put("fstat() server side failed"));
	}
	if ((ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, file, 0))
			== MAP_FAILED)
	{
		ft_putendl_fd("TEST_ERROR", fd);
		return (print_error_get_put("mmap() server side failed"));
	}
	ft_putendl_fd("TEST_OK", fd);
	send_get_server(fd, st, ptr);
	close(file);
}
