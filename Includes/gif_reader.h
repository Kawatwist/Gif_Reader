/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gif_reader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 21:09:45 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/29 01:23:19 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GIF_READER_H
# define GIF_READER_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include "error.h"

# define TRUE 1
# define DATA_HEAD 9

typedef struct      s_gif t_gif;
typedef struct      s_frame t_frame;

typedef struct      s_header
{
    unsigned char   type;
    unsigned char   len : 3;
    unsigned int    color_tab : 1;
    unsigned int    resolution : 3;
    unsigned int    sort : 1;
}                   t_header;

typedef struct      s_frame_head
{
    short           left;
    short           top;
    short           width;
    short           height;
    unsigned int    local_tab : 1;
    unsigned int    interlace : 1;
    unsigned int    sort : 1;
    unsigned int    unused : 3;
    unsigned int    size_tab : 3;
}                   t_frame_head;


struct              s_frame
{
    t_frame_head    head;
    unsigned char   *localtab;
    unsigned int    filled : 2;
    unsigned short  delay;
    unsigned short  index_transp;
    unsigned int    method : 3;
    unsigned int    input : 1;
    unsigned int    transparent : 1;
    unsigned int    unused : 3;
    unsigned char   *data;
    t_frame         *next;
};

struct              s_gif
{
    char            *path;
    unsigned char   *cm;
    int             fd;
    t_header        head;
    t_frame         *frame;
    t_frame         *current;
    unsigned int    width;
    unsigned int    height;
    int             (*type[5]) (t_gif *gif);
    int             cmlen;
    int             error_value;
};

void		checker(char *msg); // REMOVE

/*
**          Block's Function
*/
int		    get_gif_data(t_gif *gif);
int		    get_gif_app(t_gif *gif);
int		    get_gif_grap(t_gif *gif);
int		    get_gif_comment(t_gif *gif);
int		    get_gif_txt(t_gif *gif);

long		block_len(unsigned char *ptr);
void		set_zero(void *str, size_t len);
int         loop_gif(t_gif *gif);
int         get_gif_header(t_gif *gif);
void		init_gif_type(t_gif *gif);
/*
**  Basic Function To Used
*/
t_gif		*load_gif(char *path);
void        *free_gif(t_gif *gif, int error);

#endif
