/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:17:39 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 23:08:12 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

//ATTENTION '\0' DONT WORK NEED END MASK

t_lzw	*dup_block(t_lzw *src, size_t len)
{
	t_lzw *ret;
	size_t	i;

	if ((ret = malloc((sizeof(t_lzw)) * len + 1)) == NULL)
		return (NULL);
	i = -1;
	while (++i < len)
		ret[i].value = src[i].value;
	ret[i].value = END;
	return (ret);
}

t_lzw	*joinfree(t_lzw *dst, t_lzw *src, size_t n, size_t len)
{
	t_lzw		*new;
	size_t		i;
	size_t		j;
	size_t		k;

	if (!dst || !src)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	if (!(new = (t_lzw *)malloc(sizeof(t_lzw) *
					(block_lzw(src) + block_lzw(dst) + 1))))
		return (NULL);
	while ((dst[i].value != END || src[j].value != END) && j < len)
	{
		if (dst[i].value != END)
			new[k++].value = dst[i++].value;
		else if (j < len)
			new[k++].value = src[j++].value;
	}
	new[k].value = END;
	n == 1 ? free((void *)dst) : free((void *)src);
	if (n == 3)
		free((void *)dst);
	return (new);
}

long		block_lzw(t_lzw *ptr)
{
	int	i;

	i = -1;
	while (ptr[++i].value != END)
		;
	return (i);
}

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
