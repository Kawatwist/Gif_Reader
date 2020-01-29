/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 23:16:06 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/29 02:00:55 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

static int		join_block(t_gif *gif, t_frame *curr)
{
	int				pos;
	unsigned char	len;
	char			buffer[255];
	unsigned char	*new;
	unsigned char	*ptr;

	pos = 0;
	while ((read(gif->fd, &len, 1)) && len)
	{
		ptr = curr->data;
		curr->data = curr->data + pos;
		if (read(gif->fd, buffer, len) < (long)len)
			return (4);
		if ((new = malloc((curr->data - ptr) + (size_t)len)) == NULL)
			return (5);
		pos = -1;
		while (++pos < (curr->data - ptr))
			new[pos] = ptr[pos];
		curr->data != NULL ? free(curr->data) : 0;
		curr->data = new;
		new += pos;
		pos = -1;
		while (len-- && ++pos)
			new[pos] = buffer[pos];
	}
	return (0);
}

static int		get_data(t_gif *gif, t_frame *curr)
{
	if ((gif->error_value = join_block(gif, curr)))
		return (gif->error_value);
	// uncompress()
	/*
	**	LZW COMPRESSION
	*/
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
		return (5);
	if (read(gif->fd, buffer, DATA_HEAD) < DATA_HEAD)
		return (4);
	if (gif->current->filled & 0b10)
	{
		if ((gif->current->next = malloc(sizeof(t_frame))) == NULL)
			return (5);
		gif->current = gif->current->next;
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
