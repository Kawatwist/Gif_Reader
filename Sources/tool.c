/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 01:17:39 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 03:11:15 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

unsigned char	*strjoinfree(unsigned char *dst, unsigned char *src, size_t n)
{
	unsigned char	*new;
	int				i;
	int				j;
	int				k;

	if (!dst || !src)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	if (!(new = (unsigned char *)malloc(sizeof(char) *
					(block_len(src) + block_len(dst) + 1))))
		return (NULL);
	while (dst[i] || src[j])
	{
		if (dst[i])
			new[k++] = dst[i++];
		else
			new[k++] = src[j++];
	}
	new[k] = '\0';
	n == 1 ? free((void *)dst) : free((void *)src);
	if (n == 3)
		free((void *)dst);
	return (new);
}

int			cmpnstr(t_lzw *check, t_lzw *find, size_t n)
{
	size_t				i;

	i = 0;
	while (check[i].value == find[i].value && check[i].value != '\0' && find[i].value != '\0' && i < n)
		i++;
	if (i == n)
		return (0);
	return (check[i].value - find[i].value);
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
