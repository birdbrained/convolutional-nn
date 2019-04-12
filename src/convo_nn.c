#include "convo_nn.h"

typedef struct image_feature_manager_s
{
	ImageFeature *image_features;
	Uint32 max_image_features;
}ImageFeatureManager;

static ImageFeatureManager manager = { NULL, 0 };

void if_manager_close()
{
	int i = 0;

	if (manager.image_features != NULL)
	{
		for (i = 0; i < manager.max_image_features; i++)
		{
			if_free(&manager.image_features[i]);
		}
		free(manager.image_features);
	}
	memset(&manager, 0, sizeof(ImageFeatureManager));
	slog("Image Feature manager closed.");
}

void if_manager_init(Uint32 max_image_features)
{
	if (max_image_features <= 0)
	{
		slog("Warning: Cannot initialize Image Feature Manager for 0 Image Features, setting to 1024");
		max_image_features = 1024;
	}

	manager.image_features = (ImageFeature *)malloc(sizeof(ImageFeature) * max_image_features);
	if (!manager.image_features)
	{
		slog("Error: Could not allocate space for the Image Features array");
		return;
	}
	memset(manager.image_features, 0, sizeof(ImageFeature) * max_image_features);
	manager.max_image_features = max_image_features;
	atexit(if_manager_close);
	slog("Image Feature manager initialized");
}

ImageFeature * if_new()
{
	int i = 0;

	for (i = 0; i < manager.max_image_features; i++)
	{
		if (!manager.image_features[i].in_use)
		{
			memset(&manager.image_features[i], 0, sizeof(ImageFeature));
			manager.image_features[i].in_use = 1;
			return &manager.image_features[i];
		}
	}
	return NULL;
}

ImageFeature * if_new_from_surface(SDL_Surface *surface)
{
	ImageFeature *feature = NULL;
	SDL_PixelFormat *format;
	int i = 0, max = 0;
	Uint8 r = 0, g = 0, b = 0;
	Uint32 *pixels;

	if (!surface)
	{
		slog("Error: Cannot create an Image Feature from a NULL surface");
		return NULL;
	}
	feature = if_new();
	if (!feature)
	{
		return NULL;
	}

	format = surface->format;
	feature->width = surface->w;
	feature->height = surface->h;
	max = surface->w * surface->h;
	feature->pixels = (int *)malloc(sizeof(int) * max);
	pixels = (Uint32 *)surface->pixels;
	for (i = 0; i < max; i++)
	{
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		//evaulate this pixel somehow....black should be -1, white should be 1
		feature->pixels[i] = evaluate_pixel(r, g, b);
	}

	return NULL;
}

int if_free(ImageFeature *image_feature)
{
	if (!image_feature)
	{
		return 1;
	}

	if (image_feature->pixels && image_feature->width > 0 && image_feature->height > 0)
	{
		memset(image_feature->pixels, 0, sizeof(int) * image_feature->width * image_feature->height);
		free(image_feature->pixels);
	}
	memset(image_feature, 0, sizeof(ImageFeature));
	return 0;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

int evaluate_pixel(int r, int g, int b)
{
	return 0;
}

void pixel_test(SDL_Surface *surface)
{
	Uint32 *pixels;
	Uint8 *p;
	Uint32 pixel = 0;
	SDL_PixelFormat *format;
	int bpp = 0;
	int r = 0, g = 0, b = 0, i = 0, x = 0, y = 0, max = 0;
	if (!surface)
	{
		slog("c'mon matt");
		return;
	}

	bpp = surface->format->BytesPerPixel;
	format = surface->format;
	SDL_LockSurface(surface);
	pixels = (Uint32 *)surface->pixels;
	//format = SDL_GetWindowPixelFormat(gf2d_graphics_get_window());
	max = surface->w * surface->h;
	/*for (i = 0; i < max; i++)
	{
		if (i != 0)
		{
			*pixels << 8;
		}
		r = 0;
		b = 0;
		g = 0;
		SDL_GetRGB(pixels[i], format, &r, &g, &b);
		slog("Pixel (%i): r (%i) g (%i) b (%i)", i, r, g, b);
	}*/

	for (y = 0; y < surface->h; y++)
	{
		for (x = 0; x < surface->w; x++)
		{
			p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

			switch (bpp)
			{
				case 1:
					pixel = *p;
					return;
				case 2:
					pixel = *(Uint16 *)p;
					break;
				case 3:
					if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
					{
						pixel = p[0] << 16 | p[1] << 8 | p[2];
					}
					else
					{
						pixel = p[0] | p[1] << 8 | p[2] << 16;
					}
					break;
				case 4:
					pixel = *(Uint32 *)p;
					break;
				default:
					break;
			}

			SDL_GetRGB(pixel, format, &r, &g, &b);
			slog("Pixel (%i): r (%i) g (%i) b (%i)", i, r, g, b);
		}
	}
	SDL_UnlockSurface(surface);
}
