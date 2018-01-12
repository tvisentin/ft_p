/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvisenti <tvisenti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 10:26:40 by tvisenti          #+#    #+#             */
/*   Updated: 2018/01/12 12:53:33 by tvisenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_p.h"

void    usage(char *str)
{
    printf("Usage: %s <port>\n", str);
    exit (-1);
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

int     print_error(char *str)
{
    printf("Error: %s\n", str);
    return (-1);
}

/*
** Cree un server et recupere avec protobyname le protocol number avec 
** la struct protoend et la fonction getprotobyname("tcp")
*/

int     create_server(int port)
{
    int                 sock;
    struct protoent     *proto;
    struct sockaddr_in  sin;

    proto = getprotobyname("tcp");
    if (!proto)
        return (-1);
    sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (const struct  sockaddr *)&sin, sizeof(sin)) == -1)
        return print_error("Bind failed");
    listen(sock, 42);
    return (sock);
}

void     handler_serv(int fd)
{
    char                *str;
    char                *pwd;
    char                *buf;
    
    pwd = malloc(sizeof(char) * UCHAR_MAX);
    getcwd(pwd, UCHAR_MAX);
    while (42)
    {
        if (get_next_line(fd, &buf) > 0)
        {
            str = ft_strdup(buf);
            ft_putstr("Buf: ");
            ft_putendl(str);
            ft_putendl("---------");
            if (ft_strncmp(str, "ls", 2) == 0 && ft_strlen(str) >= 2)
                cmd_ls(fd, &str[3], pwd);
            else if (ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) > 3 && str[2] == ' ')
                cmd_cd(fd, &str[3], pwd);
            else if (ft_strcmp(str, "quit") == 0 && ft_strlen(str) == 4)
                return ;
            else if (ft_strcmp(str, "pwd") == 0 && ft_strlen(str) == 3)
                cmd_pwd(fd);
            else if (ft_strncmp(str, "mkdir", 5) == 0 && ft_strlen(str) > 6 && str[5] == ' ')
                cmd_mkdir(fd, &str[6]);
            else
                print_fd_err("-> ERROR: Command not found", fd);
            ft_putendl("---------");
            free(str);
        }
    }
}

int     accept_fork(unsigned int sock)
{
    int                 cs;
    struct sockaddr_in  *csin;
    unsigned int        cslen;    
    pid_t               pid;
    int                 sock_client;

    while (42)
    {
        if ((sock_client = accept(sock, (struct sockaddr*)&csin, &cslen)))
        {
            if ((pid = fork()) == -1)
                return print_error("Fork failed");
            else if (pid == 0)
            {
                handler_serv(sock_client);
                close(sock_client);
                break ;
            }
        }
        else
            return print_error("Accept failed");
    }
    close(cs);
    return (1);
}

int     main(int ac, char **av)
{
    int                 port;
    unsigned int        sock;


    if (ac != 2)
        usage(av[0]);
    port = atoi(av[1]);
    if ((sock = create_server(port)) == -1)
        return (-1);
    if ((accept_fork(sock)) == -1)
        return (-1);
    close(sock);
    return (0);
}