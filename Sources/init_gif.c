/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gif.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 23:09:43 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 23:08:32 by lomasse          ###   ########.fr       */
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
			return (4); // ! MALLOC
		free(buffer);
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
		return (4); // MALLOC
	free(buffer);
	if ((gif->error_value = reach_sub_block(gif)))
		return (gif->error_value);
	return (0);
}
/*
**	Block Terminator isnt include in the len => +1
*/
int		get_gif_grap(t_gif *gif)
{
	unsigned char	len;
	char			buffer[256];

	printf("Graphique Block here\n");
	if (read(gif->fd, &len, 1) < 1)
		return (4);
	printf("Len of this bloc [%d]\n", len);
	if (read(gif->fd, buffer, (size_t)len + 1) < (long)len + 1)
		return (4);
	printf("Buffer Load sucessfully\n");
	if (gif->current->filled & 0b11)
	{
		printf("The Frame is already Used, We need to Switch\n");
		if ((gif->current->next = malloc(sizeof(t_frame *))) == NULL)
			return (5);
		printf("New Frame Mallocated\n");
		gif->current = gif->current->next;
		gif->current->next = NULL;
		gif->current->data = NULL;
		printf("Pointer of Frame setup\n");
	}
	printf("Let's Fill the new head of the frame\n");
	gif->current->transparent = buffer[0] & 0b1;
	gif->current->input = buffer[0] & 0b10;
	gif->current->method = buffer[0] & 0b11100;
	gif->current->unused = buffer[0] & 0b11100000;
	gif->current->delay = buffer[1] + buffer[2];
	gif->current->index_transp = buffer[3];
	gif->current->filled = 0b1;
	if (buffer[(size_t)len])
		return (4);
	printf("The frame is Initilized correctly\n");
	return (0);
}

int		get_gif_comment(t_gif *gif)
{
	(void)gif;
	if ((gif->error_value = reach_sub_block(gif)))
		return (gif->error_value);
	return (0);
}

int		get_gif_txt(t_gif *gif)
{
	(void)gif;
	if ((gif->error_value = reach_sub_block(gif)))
		return (gif->error_value);
	return (0);
}

void		init_gif_type(t_gif *gif)
{
	gif->type[0] = get_gif_data;
	gif->type[1] = get_gif_app;
	gif->type[2] = get_gif_grap;
	gif->type[3] = get_gif_comment;
	gif->type[4] = get_gif_txt;
	gif->type[5] = get_gif_code_data;
}
