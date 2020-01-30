/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gif_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 22:58:35 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 01:58:03 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

/*
**	Find Order :
**
**	Data
**	Application Extension
**	Graphics Control Extension
**	Comment Extension
**	Plain Text Extension
**	Image data color code
**
**	3B is The End Of File
*/

int			find_block_type(t_gif *gif, int *find)
{
	unsigned char	val[2];

	*find = 99;
	if (read(gif->fd, &val, 1) < 1)
		return (4);
	if (*val == 0x2C || *val == 0x3B || *val == 0x2C)
	{
		val[0] == 0x2C ? *find = 0 : 0;
		val[0] == 0x02 ? *find = 5 : 0;
		val[0] == 0x3B ? *find = 6 : 0;
		return (0);
	}
	if (*val == 0x21)
	{
		if (read(gif->fd, &val, 1) < 1)
			return (3);
		val[0] == 0xFF ? *find = 1 : 0;
		val[0] == 0xF9 ? *find = 2 : 0;
		val[0] == 0xFE ? *find = 3 : 0;
		val[0] == 0x01 ? *find = 4 : 0;
	}
	if (*find == 99)
		return (3);
	return (0);
}

int			loop_gif(t_gif *gif)
{
	int		parse;

	parse = 0;
	while (TRUE)
	{
		if ((gif->error_value = find_block_type(gif, &parse)))
			return (gif->error_value);
		if (parse == 6)
			return (0);
		gif->type[parse] (gif);
	}
	return (0);
}
