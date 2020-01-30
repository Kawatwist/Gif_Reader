/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 18:56:39 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/30 04:26:01 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"

/*
**	Create a type of 9bits len
*/
/*
**	The New type will be used to reach 9 per 9 bits the src string
**
**	/!\ The dest string need to be malloced before to be used
**	(Width * Height)
*/
// int			compress_lzw(t_lzw *src, char *dest, long len, int nb)
// {
// 	long	index;
// 	char	*(dictionnary[256]);
// 	char	pos;

// 	index = -1;
// 	pos = 0;
// 	dictionnary[0] = NULL;
// 	while (index < len)
// 	{
// 		/*
// 		**	Check si 0b100000000 de value est actif
// 		**	To know if the vale is a reference to the dictionary
// 		*/
// 		if (!(src[index].type) && nb)
// 		{
// 			if ((dictionnary[(int)pos] = malloc(sizeof(char) * 2)) == NULL)
// 				return (1);
// 			for (int i = 0; i < 2 ; i++)
// 				dictionnary[(int)pos] = src[index].value;
// 			pos += 1;
// 			nb -= 1;
// 			dictionnary[(int pos)] = NULL;
// 		}
// 	}
// 	return (0);
// }

#include <stdio.h>
void		show_dico(t_lzw dico[256][256], int nb)
{
	int i;
	int j;

	i = -1;
	while (++i < nb)
	{
		j = -1;
		printf("Index [%d]:\n", i);
		while (dico[i][++j].value != END)
			printf("%c", dico[i][j].value);
		printf("\n");
	}
}

int			add_dico(t_lzw all[256][256], t_lzw *dico, t_lzw *str, int len)
{
	int	i;
	int	pos;
	int	tmp;

	i = -1;
	tmp = 0;
	while (++i < len)
	{
		if (str[i].value < LZW)
			dico[i + tmp].value = str[i].value;
		else
		{
			pos = -1;
			while (all[str[i].value - 257][++pos].value != END)
			{
				printf("[%c]\n", all[str[i].value - 257][++pos].value);
				dico[i + tmp].value = all[str[i].value - 257][++pos].value;
				tmp += pos;
			}
		}

	}
	dico[i].value = END;
	return (0);
}

int			set_dico(t_lzw dico[256][256], t_lzw *str, int nb)
{
	int		i;
	int		post;

	i = -1;
	post = 1;
	while (++i < nb)
	{
		if (!cmpnstr(dico[i], &str[-post], post + 1)) // CHECK IF EXIST
		{
			post++;
			i = -1;
		}
	}
	add_dico(dico, dico[nb], &(str[-post]), post + 1);
	return (0);
}


int			uncompress_lzw(t_lzw *src, unsigned char *dest, int nb)
{
	t_lzw			dictionary[256][256];
	int				dico;
	int				index;
	int				index2;
	int				pos;

	(void)nb;
	index = 0;
	dico = 0;
	pos = 1;
	src[0].value = 'T';
	src[1].value = 'O';
	src[2].value = 'B';
	src[3].value = 'E';
	src[4].value = 'O';
	src[5].value = 'R';
	src[6].value = 'N';
	src[7].value = 'O';
	src[8].value = 'T';
	src[9].value = 257;
	src[10].value = 259;
	src[11].value = 261;
	src[12].value = 266;
	src[13].value = 260;
	src[14].value = 262;
	src[15].value = 264;
	src[16].value = END;
	dest[0] = src[0].value;
	while (src[++index].value != END)
	{
		printf("%d\n Dest : \t", src[index].value);
		for (int i = 0; dest[i];i++)
			printf("%c", dest[i]);
		printf("\n");
		if (!set_dico(dictionary, &(src[index]), dico))
			dico++;
		if (src[index].value > LZW)
		{
			index2 = -1;
			while (dictionary[src[index].value - 257][++index2].value != END)
			{
				printf("pos : %d\n", pos);
				dest[pos] = (dictionary[src[index].value - 257][index2].value & 0xFFFFFFFF);
				pos++;
			}
		}
		else
		{
			dest[pos] = src[index].value;
			pos++;
		}

	}
	for (int i = 0; dest[i];i++)
		printf("%c", dest[i]);
	printf("\n");
	show_dico(dictionary, 9);
	return (0);
}
