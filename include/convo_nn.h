#ifndef __CONVO_NN__
#define __CONVO_NN__

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gf2d_graphics.h"
#include "simple_logger.h"

typedef struct image_feature_s
{
	Uint8 in_use;
	int *pixels;
	Uint32 width;
	Uint32 height;
}ImageFeature;

void if_manager_init(Uint32 max_image_features);
ImageFeature *if_new();
ImageFeature *if_new_from_surface(SDL_Surface *surface);
int if_free(ImageFeature *image_feature);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int evaluate_pixel(int r, int g, int b);

void pixel_test(SDL_Surface *surface);

#endif // ! __CONVO_NN__
