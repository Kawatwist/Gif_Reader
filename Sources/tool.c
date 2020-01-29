/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:17:39 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/29 01:22:45 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

long		block_len(unsigned char *ptr)
{
	unsigned char	*start;

	start = ptr;
	while (*ptr)
	{
		if (!(*ptr + 1))
			return ((ptr - start) + 1);
		if (!(*ptr + 2))
			return ((ptr - start) + 2);
		if ((*ptr + 3))
			return ((ptr - start) + 3);
		ptr += 4;
	}
	return (ptr - start);
}
