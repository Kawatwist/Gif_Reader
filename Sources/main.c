/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 21:03:54 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/31 21:38:51 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include "SDL.h"

int		main(int argc, char **argv)
{
	SDL_Window	*window;
	SDL_Renderer *rend;
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	t_gif		*gif;
	SDL_Event	ev;

	if (argc < 2)
		return (0);

	SDL_Init(SDL_INIT_EVERYTHING);
/*
**	Load Gif's Data
*/
	if ((gif = load_gif(argv[1])) == NULL)
		return (0);
	printf("Yay We got one !\n");
/*
**	Init Sdl
*/
	window = SDL_CreateWindow("Show gif file", 0, 0, 1000, 1000, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (gif->frame->pxl == NULL)
	{
		printf("NULL !\n");
		exit(0);
	}
	surface = SDL_CreateRGBSurfaceWithFormatFrom(gif->frame->pxl, gif->frame->head.width, gif->frame->head.height, 24, gif->frame->head.width * 3, SDL_PIXELFORMAT_BGR24);
	texture = SDL_CreateTextureFromSurface(rend, surface);
	SDL_RenderCopy(rend, texture, NULL, NULL);
/*
**	Draw on Screen Result
*/
	SDL_RenderPresent(rend);
	SDL_UpdateWindowSurface(window);
	SDL_PollEvent(&ev);
	SDL_Delay(2000);
/*
**	Clear Sdl Memory
*/
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
/*
**	NEED TO Free the Gif's Struct
*/
	return (0);
}


