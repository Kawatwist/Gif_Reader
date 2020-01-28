/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gif.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 23:09:43 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/29 00:10:37 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

int		reach_sub_block(t_gif *gif)
{
	unsigned char	len;
	char			*buffer;

	while (TRUE)
	{
		if (read(gif->fd, &len, 1) < 1)
			return (4);
		if (len == 0)
			break ;
		if ((buffer = malloc((size_t)len)) == NULL)
			return (5);
		if (read(gif->fd, buffer, (size_t)len) < (long)len)
			return (4);
	}
	return (0);
}

int		get_gif_app(t_gif *gif)
{
	unsigned char	len;
	char			*buffer;

	if (read(gif->fd, &len, 1) < 1)
		return (4);
	if ((buffer = malloc((size_t)len)) == NULL)
		return (5);
	if (read(gif->fd, buffer, (size_t)len) < (long)len)
		return (4);
	if ((gif->error_value = reach_sub_block(gif)))
		return (gif->error_value);
	return (0);
}

int		get_gif_grap(t_gif *gif)
{
	char			*buffer;

	if ((buffer = malloc(GRAPH_LEN)) == NULL)
		return (5);
	if (read(gif->fd, buffer, GRAPH_LEN) < GRAPH_LEN)
		return (4);
	return (0);
}

int		get_gif_comment(t_gif *gif)
{
	(void)gif;
	return (0);
}

int		get_gif_txt(t_gif *gif)
{
	(void)gif;
	return (0);
}

void		init_gif_type(t_gif *gif)
{
	gif->type[0] = get_gif_data;
	gif->type[1] = get_gif_app;
	gif->type[2] = get_gif_grap;
	gif->type[3] = get_gif_comment;
	gif->type[4] = get_gif_txt;
}
