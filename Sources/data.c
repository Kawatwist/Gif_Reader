/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 23:16:06 by lomasse           #+#    #+#             */
/*   Updated: 2020/02/01 06:26:25 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include <stdio.h>

static int		join_block(t_gif *gif, t_frame *curr)
{
	t_lzw			end;
	unsigned char	len;
	char			buffer[256];

	curr->data = NULL;
	printf("Starting the loop of join block (fd : %d)\n", gif->fd);
	while ((read(gif->fd, &len, 1)) && len)
	{
		printf("Buffer Read[%d]\n", len);
		if (read(gif->fd, buffer, len) < (long)len)
			return (4);
		printf("Buffer Read Done (%p)\n", curr->data);
		if (curr->data == NULL)
			curr->data = dup_block((t_lzw *)buffer, len);
		else
			curr->data = joinfree((t_lzw *)curr->data, (t_lzw *)buffer, 1, len);
		printf("Memory Store\n");
	}
	printf("Ending the loop of join block\n");
	end.value = END;
	curr->data = joinfree((t_lzw *)curr->data, &(end), 1, 1);
	printf("Exiting join Function\n");
	return (0);
}

static void		set_color(t_gif *gif, t_frame *curr, unsigned char *base, unsigned char *pxl, size_t len)
{
	unsigned int	i;

	i = -1;
	printf("Set_color~\n");
	while (++i < len)
	{
		if (curr->head.local_tab)
		{
			pxl[(i * 3) + 0] = ((char *)&curr->localtab[base[i] * 3])[0];
			pxl[(i * 3) + 1] = ((char *)&curr->localtab[base[i] * 3])[1];
			pxl[(i * 3) + 2] = ((char *)&curr->localtab[base[i] * 3])[2];
		}
		else
		{
			if (base[i] >= (gif->cmlen + 1) * (gif->cmlen + 1))
				printf("TROP LONG !\n");
			if (base[i] > gif->cmlen)
				printf("Probleme de taille Global Color Map (%d / %d)\n", base[i], gif->cmlen);
			else
			{
				pxl[(i * 3) + 0] = ((char *)&gif->cm[base[i]])[0];
				pxl[(i * 3) + 1] = ((char *)&gif->cm[base[i]])[1];
				pxl[(i * 3) + 2] = ((char *)&gif->cm[base[i]])[2];
				// printf("Pxl : [%d] => [%#x]\t[%#x]\t[%#x]\n", i, pxl[(i * 3) + 0],pxl[(i * 3) + 1],pxl[(i * 3) + 2]);
			}
		}
	}
	printf("End Set_color~ [%d]\n", i);
}

int			get_data(t_gif *gif)
{
	char		nb;
	size_t		i;
	size_t		uncompress;
	t_frame		*curr;

	uncompress = 0;
	curr = gif->current;
	printf("Welcome to the data section (Fd : %d)\n", gif->fd);
	if (read(gif->fd, &nb, 1) < 1)
		return (4);
	if ((gif->error_value = join_block(gif, curr)))
		return (gif->error_value);
	printf("Join Block Done\n");
	if (curr->data == NULL)
		return (5);
	printf("I'll mallocated : %d (Width : %d\tHeigth : %d)\n", curr->head.width * curr->head.height, curr->head.width, curr->head.height);
	if ((curr->uncompress = malloc((size_t)(curr->head.width * curr->head.height))) == NULL)
		return (5);
	i = -1;
	while (++i < (size_t)(curr->head.width * curr->head.height))
		curr->uncompress[i] = 0;
	printf("Let's go to uncompress this shit\n");
	if ((gif->error_value = uncompress_lzw((t_lzw *)curr->data, curr->uncompress, nb, &uncompress)))
		return (gif->error_value);
	printf("Wow ! Well done\n");
	free(curr->data);
	if ((curr->pxl = malloc((size_t)(curr->head.width * curr->head.height))) == NULL)
		return (5);
	i = -1;
	while (++i < (size_t)(curr->head.width * curr->head.height))
		curr->pxl[i] = 0;
	printf("Pixel Malloced and ready to used\n");
	set_color(gif, curr, curr->uncompress, curr->pxl, uncompress);
	printf("Uncompressed data convert in color pixel\n");
	free(curr->uncompress);
	printf("%d\n", gif->height * gif->width);
	/*
	**	LZW UNCOMPRESSION
	*/
	curr->filled |= 0b10;
	return (0);
}

static int		get_color_map(t_gif *gif, t_frame *curr)
{
	int	len;

	len = 3 * (1 << (curr->head.size_tab + 1)); // Not Sure
	printf("Len of the color map [%d]\n", len);
	if ((curr->localtab = malloc((size_t)len)) == NULL)
		return (5);
	if (read(gif->fd, curr->localtab, (size_t)len) < (long)len)
		return (4);
	printf("Color map get");
	return (0);
}
/*
**	Image Descriptor
*/
int		get_gif_data(t_gif *gif)
{
	(void)gif;
	char			buffer[10];

	printf("Data ! (Fd : %d)\n", gif->fd);
	if (read(gif->fd, buffer, DATA_HEAD) < DATA_HEAD)
		return (4);
	if (gif->current->filled & 0b10)
	{
		if ((gif->current->next = malloc(sizeof(t_frame *))) == NULL)
			return (5);
		gif->current = gif->current->next;
		gif->current->next = NULL;
		gif->current->data = NULL;
		printf("Frame Resetup(Fd : %d)\n", gif->fd);
	}
	printf("Setup Header Frame\n");
	gif->current->head.left = buffer[0] + (buffer[1] << 8);
	gif->current->head.top = buffer[2] + (buffer[3] << 8);
	gif->current->head.width = buffer[4] + (buffer[5] << 8);
	gif->current->head.height = buffer[6] + (buffer[7] << 8);
	gif->current->head.local_tab = (buffer[8] & 0b10000000 >> 7);
	gif->current->head.interlace = (buffer[8] & 0b1000000 >> 6);
	gif->current->head.sort = (buffer[8] & 0b100000 >> 5);
	gif->current->head.unused = (buffer[8] & 0b11000 >> 3);
	gif->current->head.size_tab = buffer[8] & 0b111;
	printf("Get Color Map\n");
	if (gif->current->head.local_tab)
		get_color_map(gif, gif->current);
	if ((gif->error_value = get_data(gif)))
		return (gif->error_value);
	printf("Head data end (Fd : %d)\n", gif->fd);
	return (0);
}
