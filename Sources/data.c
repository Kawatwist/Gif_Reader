/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 23:16:06 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 03:35:32 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

static int		join_block(t_gif *gif, t_frame *curr)
{
	int				pos;
	unsigned char	len;
	char			buffer[255];

	pos = 0;
	if ((curr->data = malloc(1)) == NULL)
		return (5);
	while ((read(gif->fd, &len, 1)) && len)
	{
		curr->data = curr->data + pos;
		if (read(gif->fd, buffer, len) < (long)len)
			return (4);
		curr->data = strjoinfree(curr->data, (unsigned char *)buffer, 1);
	}
	return (0);
}

static int		get_data(t_gif *gif, t_frame *curr)
{
	size_t		i;

	if ((gif->error_value = join_block(gif, curr)))
		return (gif->error_value);
	if ((curr->pxl = malloc((size_t)(curr->head.width * curr->head.height))) == NULL)
		return (5);
	i = -1;
	while (++i < (size_t)(curr->head.width * curr->head.height))
		curr->pxl[i] = 0;
	if (curr->data == NULL)
		return (5);
	if ((gif->error_value = uncompress_lzw((t_lzw *)curr->data, curr->pxl, curr->head.sort)))
		return (gif->error_value);
	/*
	**	LZW UNCOMPRESSION
	*/
	curr->filled |= 0b10;
	return (0);
}

static int		get_color_map(t_gif *gif, t_frame *curr)
{
	int	len;

	len = 3 * (1 << (curr->head.size_tab + 1)); // Not Sure
	if ((curr->localtab = malloc((size_t)len)) == NULL)
		return (5);
	if (read(gif->fd, curr->localtab, (size_t)len) < (long)len)
		return (4);
	return (0);
}

int		get_gif_data(t_gif *gif)
{
	(void)gif;
	char			*buffer;

	checker("Data !\n");
	if ((buffer = malloc(DATA_HEAD)) == NULL)
		return (5); // FREE
	if (read(gif->fd, buffer, DATA_HEAD) < DATA_HEAD)
		return (4);
	if (gif->current->filled & 0b10)
	{
		if ((gif->current->next = malloc(sizeof(t_frame))) == NULL)
			return (5);
		gif->current = gif->current->next;
		gif->current->next = NULL;
	}
	gif->current->head.left = buffer[0] + (buffer[1] << 8);
	gif->current->head.top = buffer[2] + (buffer[3] << 8);
	gif->current->head.width = buffer[4] + (buffer[5] << 8);
	gif->current->head.height = buffer[6] + (buffer[7] << 8);
	gif->current->head.local_tab = (buffer[8] & 0b10000000 >> 7);
	gif->current->head.interlace = (buffer[8] & 0b1000000 >> 6);
	gif->current->head.sort = (buffer[8] & 0b100000 >> 5);
	gif->current->head.unused = (buffer[8] & 0b11000 >> 3);
	gif->current->head.size_tab = buffer[8] & 0b111;
	if (gif->current->head.local_tab)
		get_color_map(gif, gif->current);
	gif->current->data = NULL;
	get_data(gif, gif->current);
	return (0);
}

int			get_gif_code_data(t_gif *gif)
{
	(void)gif;
	printf("Salam\n");
	return (0);
}
