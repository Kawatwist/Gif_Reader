/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gif_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 21:17:14 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 22:49:55 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

void		checker(char *msg)
{
	printf("%s\n", msg);
}

void		set_zero(void *str, size_t len)
{
	while(--len > -1)
		((char *)str)[len] = 0;
}

void		*free_gif(t_gif *gif, int error)
{
	static char	*tmp[6] = {NULL, OP, HD, UBK, READ_FAIL, MAL_FAIL};
	/*
	**	Free The Gif Struct
	*/
	(void)gif;
	printf("Get out ![%d]\n", gif->error_value);
	printf("%s\n", tmp[error]);
	return (NULL);
}

t_gif		*load_gif(char *path)
{
    t_gif	*gif;

    if ((gif = malloc(sizeof(t_gif))) == NULL)
		return (NULL);
/*
**	Bzero to initialize the struct
*/
	set_zero(gif, sizeof(t_gif));
	if ((gif->fd = open(path, O_RDONLY)) < 0 || read(gif->fd, NULL, 0))
		return (free_gif(gif, 1));
	if ((gif->frame = malloc(sizeof(t_frame *))) == NULL)
		return (free_gif(gif, 5));
	set_zero(gif->frame, sizeof(t_frame));
	gif->current = gif->frame;
	gif->current->next = NULL;
/*
**	Parsing The Header File
*/
	if ((gif->error_value = get_gif_header(gif)))
		return (free_gif(gif, gif->error_value));
	init_gif_type(gif);
	if ((gif->error_value = loop_gif(gif)))
		return (free_gif(gif, gif->error_value));
    return (gif);
}
