/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasse <lomasse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 21:03:54 by lomasse           #+#    #+#             */
/*   Updated: 2020/01/28 21:30:10 by lomasse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gif_reader.h"
#include "SDL.h"

int		main(int argc, char **argv)
{
	SDL_Window	*window;
	// SDL_Surface	*surface;
	// SDL_Texture	*texture;
	SDL_Renderer *rend;
	t_gif		*gif;
	SDL_Event	ev;
	
	if (argc < 2)
		return (0);
/*
**	Load Gif's Data
*/
	if ((gif = load_gif(argv[1])) == NULL)
		return (0);
/*
**	Init Sdl
*/
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Show gif file", 0, 0, 800, 800, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
/*
**	Draw on Screen Result
*/
	// surface = SDL_CreateRGBSurfaceWithFormatFrom(tga->data, tga->w, tga->h, 32, tga->w * 4, SDL_PIXELFORMAT_ARGB32);
	// texture = SDL_CreateTextureFromSurface(rend, surface);
	// SDL_RenderCopy(rend, texture, NULL, NULL);
	SDL_RenderPresent(rend);
	SDL_UpdateWindowSurface(window);
	SDL_PollEvent(&ev);
	SDL_Delay(1000);


	// SDL_DestroyTexture(texture);
	// SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}