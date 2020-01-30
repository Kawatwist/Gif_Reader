/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 18:56:39 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 22:12:08 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

/*
**	Create a type of 9bits len
*/

#include <stdio.h>

void		show_one_dico(t_lzw dico[256])
{
	int	i;

	i = -1;
	while (dico[++i].value != END)
		printf("%c", dico[i].value);
	printf("]\n");
}

void		show_dico(t_lzw dico[256][256], int nb)
{
	int i;
	int j;

	i = -1;
	while (++i < nb)
	{
		j = -1;
		printf("Index [%d]:\t[", i);
		while (dico[i][++j].value != END)
			printf("%c", dico[i][j].value);
		printf("]\n");
	}
}

/*
**	Check if the current word already exist in the dictionary
*/
int			in_dico(t_lzw dico[256][256], t_lzw *src, int len, int max)
{
	int nb;
	int	i;

	nb = 0;
	while (nb < max)
	{
		i = 0;
		while (src[i].value == dico[nb][i].value && i < len)
			i++;

		if (i == len - 1)
			return (1);
		else
			nb++;
	}
	return (0);
}

/*
**	Add The a new word to the dictionary
*/

int			add_dico(t_lzw dico[256][256], t_lzw *src, int nb)
{
	int i;
	int	tmp2;
	int	tmp;
	int	len;

	i = 0;
	tmp2 = 0;
	len = -1;
	while (src[i - 1].value != END && ++len < 2)
	{
		if (src[len].value > END)
		{
			tmp = src[len].value - 257;
			tmp2 = 0;
			if (tmp < nb && in_dico(dico, dico[nb], i + 1, nb - 1))
			{
				while (tmp < nb && in_dico(dico, dico[nb], i + 1, nb - 1))
					dico[nb][i++].value = dico[tmp][tmp2++].value;
				if (!in_dico(dico, dico[nb], i, nb - 1))
					dico[nb][i].value = dico[tmp][tmp2++].value;
			}
			else if (!in_dico(dico, dico[nb], i + 1, nb - 1))
				dico[nb][i++].value = dico[tmp][tmp2++].value;
		}
		else
		{
			dico[nb][i].value = src[i].value;
			i++;
		}
	}
	dico[nb][i].value = END;
	return (0);
}

/*
**	Fill The Dest's String
*/

int			place_dest(t_lzw dico[256][256], t_lzw *src, unsigned char *dest)
{
	int	i;

	i = 0;
	if (src[0].value < END)
		dest[0] = src[0].value;
	else if (src[0].value > END)
	{
		i = -1;
		while (dico[src[0].value - 257][++i].value != END)
			dest[i] = dico[src[0].value - 257][i].value;
		i -= 1;
	}
	return (i + 1);
}

/*
**	Init Dictionary
*/

void		clear_dico(t_lzw dico[256][256])
{
	int	i;
	int	j;

	i = -1;
	while (++i < 256)
	{
		j = -1;
		while (++j < 256)
			dico[i][j].value = END;
	}
}

int			uncompress_lzw(t_lzw *src, unsigned char *dest, int nb, size_t *len)
{
	t_lzw			dictionary[256][256];
	int				dico;
	int				index;
	int				index2;

	(void)nb;
/*
**	Wikipedia Example
*/

	// src[0].value = 'T';
	// src[1].value = 'O';
	// src[2].value = 'B';
	// src[3].value = 'E';
	// src[4].value = 'O';
	// src[5].value = 'R';
	// src[6].value = 'N';
	// src[7].value = 'O';
	// src[8].value = 'T';
	// src[9].value = 257;
	// src[10].value = 259;
	// src[11].value = 261;
	// src[12].value = 266;
	// src[13].value = 260;
	// src[14].value = 262;
	// src[15].value = 264;
	// src[16].value = END;
	index = 0;
	index2 = 1;
	dico = 0;
	clear_dico(dictionary);
	dest[0] = src[0].value;
	while (src[++index].value != END)
	{
		if (dico < 255)
			add_dico(dictionary, &(src[index - 1]), dico);
		index2 += place_dest(dictionary, &(src[index]), &(dest[index2]));
		dico++;
	}
	*len = index2;
	// printf("%d || %d\n", index2, dico);
	// printf("Resultat : ");
	// for (int i = 0; dest[i];i++)
	// 	printf("%c", dest[i]);
	// printf("\n");
	// for (int i = 0; i < 40; i+=3)
	// 	printf("[%d]\t[%d]\t[%d]\n", dest[i + 0], dest[i + 1], dest[i + 2]);
	return (0);
}
