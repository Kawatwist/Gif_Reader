/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 22:08:48 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/31 22:05:25 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

int	get_color_tab(t_gif *gif, int	len)
{
	/*
	**	Color Map Defined
	*/
	if ((gif->cm = malloc((size_t)len)) == NULL)
		return (1);
	gif->cmlen = len;
	if (read(gif->fd, gif->cm, gif->cmlen) < gif->cmlen)
		return (4);
	/*
	**	Placed At the First Block
	*/
	return (0);
}

int	 get_gif_header(t_gif *gif)
{
	unsigned char	buf[13];

	if (read(gif->fd, buf, 13) < 13)
		return (4);
	printf("Header And Logical Screen Descriptor [%#x]\n", buf[6]);
	gif->head.type = buf[4];
	gif->width = buf[6] + (buf[7] << 8);
	gif->height = buf[8] + (buf[9] << 8);
	gif->head.len = buf[10] & 0b111;
	gif->head.sort = buf[10] & 0b1000;
	gif->head.resolution = buf[10] & 0b1110000;
	gif->head.color_tab = buf[10] & 0b10000000;
	/*
	**	Len of the Color Map is 3 * 2^(N + 1)
	**				N = gif->head.len
	*/
	if (get_color_tab(gif, (1 << (gif->head.len + 1)) * 3))
		return (4);
	return (0);
}
