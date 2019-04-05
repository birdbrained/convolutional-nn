#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_collision.h"
#include "ds_hashmap.h"
#include "ds_spatial_hash.h"
#include "ds_adj_graph.h"

int main(int argc, char * argv[])
{
	/*variable declarations*/
	int done = 0;
	const Uint8 * keys;
	Sprite *sprite = NULL;
	//char * c = "my string";
	//Hashmap * hashbrown = NULL;
	//void * data;
	//int j = 0;
	//char inputKey[512] = "bleh";
	//int inputInt = 0;

	int mx, my, i;
	float mf = 0;
	Sprite *mouse = NULL;
	Vector4D mouseColor = { 255,255,100,200 };
	Space *space = NULL;
	static Body body[10000];// not a pointer!
	Shape shape[4];// not a pointer!
				   /*program initializtion*/
	List * spaceArray[100];
	int setter = 0;

	Hashmap * spaceHashmap = NULL;
	Sprite * diagram = NULL;
	AdjGraph * graph = adjgraph_new(6);

	init_logger("gf2d.log");
	slog("---==== BEGIN ====---");
	gf2d_graphics_initialize(
		"Topological sort (look at slogs)",
		1200,
		720,
		1200,
		720,
		vector4d(0, 0, 0, 255),
		0);
	gf2d_graphics_set_frame_delay(16);
	gf2d_sprite_init(1024);
	SDL_ShowCursor(SDL_DISABLE);

	/*demo setup*/
	sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
	diagram = gf2d_sprite_load_image("images/diagram.png");

	space = gf2d_space_new_full(
		3,
		gf2d_rect(0, 0, 1200, 700),
		0.1,
		vector2d(0, 0.1),
		1,
		0.1);

	/*spaceHashmap = spatial_setup(space->bounds.w, space->bounds.h, 100, sizeof(Body));
	for (setter = 0; setter < 100; setter++)
	{
		spaceArray[setter] = gf2d_list_new();
	}*/
	graph = adjgraph_add_edge(graph, 0, 1);
	graph = adjgraph_add_edge(graph, 0, 4);
	graph = adjgraph_add_edge(graph, 1, 4);
	graph = adjgraph_add_edge(graph, 2, 1);
	graph = adjgraph_add_edge(graph, 2, 5);
	graph = adjgraph_add_edge(graph, 3, 4);
	graph = adjgraph_add_edge(graph, 4, 5);
	adjgraph_print(graph);
	adjgraph_topological_sort(graph);

	shape[0] = gf2d_shape_circle(0, 0, 5);
	shape[1] = gf2d_shape_circle(20, 0, 25);
	shape[2] = gf2d_shape_rect(-32, -32, 64, 64);
	shape[3] = gf2d_shape_rect(-16, -16, 32, 32);

	//gf2d_space_add_static_shape(space,gf2d_shape_rect(200,500, 512,32));
	/* Stress test*/
	/*for (i = 0; i < 75; i++)
	{
		gf2d_body_set(
			&body[i],
			i,
			"body",
			ALL_LAYERS,
			0,
			vector2d(
				600 + (gf2d_crandom() * 500),
				360 + (gf2d_crandom() * 300)),
			vector2d(gf2d_crandom(), gf2d_crandom()),
			10,
			1,
			1,
			&shape[i%4],
			NULL,
			NULL,
			NULL);
		gf2d_space_add_body(space, &body[i]);
		//spatial_register_body(&spaceHashmap, &body[i]);
		spatial_register_body(&spaceArray, &body[i]);
	}*/
	/* collision test*/
	//         gf2d_body_set(
	//             &body[0],
	//             "Body A",
	//             ALL_LAYERS,
	//             0,
	//             vector2d(100,300),
	//             vector2d(-1,0),
	//             1000,
	//             0,
	//             0,
	//             &shape[2],
	//             NULL,
	//             NULL,
	//             NULL);
	//          gf2d_space_add_body(space,&body[0]);
	//         gf2d_body_set(
	//             &body[1],
	//             "Body B",
	//             ALL_LAYERS,
	//             0,
	//             vector2d(600,340),
	//             vector2d(1,1),
	//             10,
	//             1,
	//             1,
	//             &shape[0],
	//             NULL,
	//             NULL,
	//             NULL);
	//         gf2d_space_add_body(space,&body[1]);
	//*/

	/*slog("Enter some key/value pairs (quit 0) to quit");
	while (1)
	{
		scanf("%s %i", inputKey, &inputInt);
		slog("Entered: %s %i", inputKey, inputInt);
		if (strncmp("quit", inputKey, 512) == 0)
			break;
		slog("Inserting: %s %i", inputKey, inputInt);
		hashmap_insert(&hashbrown, &inputKey, inputInt, sizeof(int));
	}

	hashmap_print(hashbrown);

	slog("Enter a key to search for (quit to quit)");
	while (1)
	{
		scanf("%s", inputKey);
		if (strncmp("quit", inputKey, 512) == 0)
		{
			slog("Stopping the search.");
			break;
		}
		slog("Searching for %s...", inputKey);
		data = hashmap_get_data(hashbrown, inputKey);
		if (data != NULL)
			slog("Found data (%i) at key (%s)", (int)data, inputKey);
		else
			slog("Could not find key (%s) in hashmap...", inputKey);
	}

	slog("Enter a key to delete (quit to quit)");
	while (1)
	{
		scanf("%s", inputKey);
		if (strncmp("quit", inputKey, 512) == 0)
		{
			slog("Stopping deletion");
			break;
		}
		slog("Trying to delete key (%s)...", inputKey);
		data = hashmap_delete(&hashbrown, inputKey);
		if (data != NULL)
		{
			slog("Deleted data (%i) at key (%s)\nThe hashmap now has:\n========================", (int)data, inputKey);
			hashmap_print(hashbrown);
		}
		else
			slog("Could not find key (%s) in hashmap...", inputKey);
	}*/
	/*c = (char)37;
	data = hashmap_get_data(hashbrown, &c);
	if (data != NULL)
		slog("trying value (%i)", (int)data);
	else
		slog("bad key");*/
	
	slog("done creating shapes");
	//hashmap_print(spaceHashmap);

	/*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
										   /*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;


		gf2d_graphics_clear_screen();// clears drawing buffers
									 // all drawing should happen betweem clear_screen and next_frame
									 //backgrounds drawn first
		gf2d_sprite_draw_image(sprite, vector2d(0, 0));

		//spatial_update(space, &spaceHashmap, space->bounds.w, space->bounds.h, 100, sizeof(Body));
		//spatial_update(space, spaceArray);
		//gf2d_space_update(space, spaceArray);

		//gf2d_space_draw(space);
		//UI elements last
		gf2d_sprite_draw(
			mouse,
			vector2d(mx, my),
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			(int)mf);

		gf2d_sprite_draw_image(diagram, vector2d(400, 200));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
		//slog("Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
	}

	gf2d_space_free(space);
	hashmap_clear(spaceHashmap);
	slog("---==== END ====---");
	return 0;
}
/*eol@eof*/