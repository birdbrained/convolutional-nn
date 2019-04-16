#ifndef __CONVO_NN__
#define __CONVO_NN__

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gf2d_graphics.h"
#include "simple_logger.h"

#define DEFAULT_MAX_IMAGE_FEATURES 1024

typedef struct image_feature_s
{
	Uint8 in_use;
	float *pixels;
	Uint32 width;
	Uint32 height;
}ImageFeature;

/**
 * @brief Initializes the Image Feature Manager
 * @param The maximum number of image features to reserve space for, will default to DEFAULT_MAX_IMAGE_FEATURES if 0 is passed in
 */
void imf_manager_init(Uint32 max_image_features);

/**
 * @brief Finds and returns memory for a new ImageFeature from the manager
 * @returns A pointer to a new ImageFeature on success; NULL if all memory slots are taken
 */
ImageFeature *imf_new();

/**
 * @brief Calls imf_new() to get a new ImageFeature, then allocates memory for the pixels array
 * @param width The width of the pixels array
 * @param height The height of the pixels array
 * @returns A pointer to a new ImageFeature on success; NULL if all memory slots are taken or if memory could not be allocated to the pixels array
 */
ImageFeature *imf_new_with_pixels_array(Uint32 width, Uint32 height);

ImageFeature *imf_new_from_surface(SDL_Surface *surface);

/**
 * @brief Frees an ImageFeature from memory, should probably not be called manually, and let the destructor handle it
 * @param image_feature The pointer to the ImageFeature to free
 */
int imf_free(ImageFeature *image_feature);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/**
 * @brief Initially evaluates a pixel from the image to create the base feature array
 * @param r The red value of the pixel
 * @param g The green value of the pixel
 * @param b The blue value of the pixel
 * @returns Currently: -1 if the pixel is black, 1 otherwise
 */
int evaluate_pixel(int r, int g, int b);

ImageFeature *imf_determine_feature_weights(SDL_Surface *surface);

void log_pixels(ImageFeature *image_feature);

#endif // ! __CONVO_NN__
