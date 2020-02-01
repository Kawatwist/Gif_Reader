/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gif_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 22:58:35 by lomasse           #+#    #+#             */
/*   Updated: 2020/02/01 02:50:48 by lomasse          ###   ########.fr       */
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
	unsigned char	val;

	*find = 99;
	if (read(gif->fd, &val, 1) < 1)
		return (4);
	printf("I'll check the new block [%#x]\n", val);
	if (val == 0x2C || val == 0x3B)
	{
		val == 0x2C ? *find = 0 : 0;
		val == 0x3B ? *find = 5 : 0;
		return (0);
	}
	printf("New block ?\n");
	if (val == 0x21)
	{
		printf("The New block is ...\n");
		if (read(gif->fd, &val, 1) < 1)
			return (4);
		printf("The New Block is in the 0x21 Section ... Spoiler : [%#x]\n", val);
		val == 0xFF ? *find = 1 : 0;
		val == 0xF9 ? *find = 2 : 0;
		val == 0xFE ? *find = 3 : 0;
		val == 0x01 ? *find = 4 : 0;
	}
	if (*find == 99)
		return (3);
	return (0);
}

int			loop_gif(t_gif *gif)
{
	int		parse;

	parse = 99;
	while (TRUE)
	{
		if ((gif->error_value = find_block_type(gif, &parse)))
			return (gif->error_value);
		tmp_c();
		if (parse == 5)
			return (0);
		gif->type[parse] (gif);
	}
	return (0);
}
