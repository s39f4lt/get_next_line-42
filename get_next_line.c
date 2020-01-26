/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idunaver <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 15:49:11 by idunaver          #+#    #+#             */
/*   Updated: 2019/01/11 22:12:33 by idunaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list		*ft_list_fd(t_list **doc, const int fd)
{
	t_list *a;

	a = *doc;
	while (a)
	{
		if (a->content_size == (size_t)fd)
			return (a);
		a = a->next;
	}
	if (!(a = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (!(a->content = ft_memalloc(1)))
	{
		free(a);
		return (NULL);
	}
	a->content_size = fd;
	ft_lstadd(doc, a);
	return (a);
}

static void			ft_freelst(t_list **temp, const int fd)
{
	t_list *current;
	t_list *previous;

	current = *temp;
	previous = NULL;
	while (current)
	{
		if (current->content_size == (size_t)fd)
			break ;
		previous = current;
		current = current->next;
	}
	if (previous)
		previous->next = current->next;
	else
		*temp = current->next;
	ft_memdel(&current->content);
	free(current);
}

static int			ft_result(char **line, char *tmp, t_list *temp)
{
	if (tmp && (temp->content = ft_strchr(tmp, '\n')))
	{
		if (!(*line = ft_strsub(tmp, 0, ft_strlen(tmp)
		- ft_strlen(temp->content))))
			return (-1);
		if (!(temp->content = ft_strsub(temp->content, 1,
		ft_strlen((temp->content) + 1))))
			return (-1);
		ft_strdel(&tmp);
		return (1);
	}
	else if (tmp && *tmp)
	{
		if (!(*line = ft_strdup(tmp)))
			return (-1);
		ft_memdel(&temp->content);
		return (1);
	}
	return (0);
}

int					get_next_line(const int fd, char **line)
{
	static t_list	*doc;
	t_list			*temp;
	char			*tmp;
	char			buff[BUFF_SIZE + 1];
	size_t			bytes;

	if (read(fd, buff, 0) < 0 || !(temp = ft_list_fd(&doc, fd)))
		return (-1);
	tmp = temp->content;
	while ((bytes = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[bytes] = '\0';
		if (!(tmp = ft_strjoin(temp->content, buff)))
			return (-1);
		ft_memdel(&temp->content);
		if ((temp->content = ft_strchr(tmp, '\n')))
			break ;
		temp->content = tmp;
	}
	if (ft_result(line, tmp, temp) == 1)
		return (1);
	else if (ft_result(line, tmp, temp) == -1)
		return (-1);
	ft_freelst(&doc, fd);
	return (0);
}
