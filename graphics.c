#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#include "terrain_generator.h"

/*TODO:
	- map resolution scaling. resize by averaging m x m pixels.
	- map trimming.
	- terrain smoothing.
	- terrain height gradient x grade, y grade
*/ 

void set_pixel(uint32_t* pixel, uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) {
	(*pixel) = 0;
	(*pixel) = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

// Terrain map meta-data to help displaying.
struct meta_data {
	int length_x;
	int length_y;
	int height_min;
	int height_max;
};

int height_range(int* arr, struct meta_data* data) {
	int min = INT_MAX;
	int max = INT_MIN;
	
	for(int i = 0; i < ((data->length_x) * (data->length_y)) ; ++i) {
		if (arr[i] < min) min = arr[i];
		if (arr[i] > max) max = arr[i];
	}
	if ((min == INT_MAX) || (max == INT_MIN)) return 1;
	data->height_min = min;
	data->height_max = max;
	return 0;
}

int main(int argc, char* argv[]) {
	// TODO: getopt()
	bool gui = false;
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-g")) gui = true; 
	}

	// Map parameters.
	struct terrain_parameters map_params;
	init_terrain_parameters(&map_params);

	bool SDL_error = false;
	//if (gui) {
		//TODO: get params from window
	//}else {
		for (int i = 1; i < argc; ++i) {
			if (!strcmp(argv[i], "-r")) map_params.resolution = atoi(argv[i + 1]);
			if (!strcmp(argv[i], "-h")) map_params.height = atoi(argv[i + 1]); 
			if (!strcmp(argv[i], "-n")) map_params.noise = atoi(argv[i + 1]); 
			if (!strcmp(argv[i], "-s")) map_params.seed = atoi(argv[i + 1]); 
		}
	//};

	int n = map_params.resolution;
	height_t* height_map = malloc(n * n * sizeof(height_t));
	map_generate(height_map, map_params, 0, 0);
	
	//TODO:error checking.
	if (gui) {
		// Display map.
		SDL_Window* window = NULL;
		//if (SDL_Init(SDL_INIT_VIDEO) < 0) SDL_error = true;
		window = SDL_CreateWindow("Terrain Generator", 
		  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_SHOWN);
		
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_Texture* texture = SDL_CreateTexture(renderer,
		  SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 1366, 768);

// --- Temporary
		/*
		struct meta_data map_data;
		map_data.length_x = 1366;
		map_data.length_y = 768;
		height_range(height_map, &map_data);

		float rescale = 255.0 / ((float) (map_data.height_max - map_data.height_min));

		if (map_data.height_min < 0) {
			for (int i = 0; i < 1366; ++i) {
				for(int j = 0; j < 768; ++j){
					height_map[i * 1366 + j] -= map_meta.height_min;
					height_map[i * 1366 + j] = (int) (((float) height_map[i * 1366 + j]) * rescale);
				}
			}
		};

*/
		uint32_t* display = map_trim(height_map, map_params, 1366, 768);
		//uint32_t* display = malloc(1366 * 768 * sizeof(uint32_t));
/*
		for (int i = 0; i < (1366 * 768); ++i) {
			set_pixel(d1 + i, 255, 150, 150, 0);
			if (height_map[i] < 50)  {
			}else if (height_map[i] < 100) {
			}else if (1height_map[i] ) < 150) {
			}else if (1height_map[i] ) < 200) {
			}else {
			};
		}
*/
// ---
		SDL_UpdateTexture(texture, NULL, display, 1366 * sizeof(int));

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		bool active_window = true;
		SDL_Event event;
		while (active_window) {
			//SDL_WaitEvent(&event);
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) active_window = false;
			};

			SDL_Delay(50);
		}

		SDL_DestroyWindow(window);
		SDL_Quit();
		if (SDL_error) {
			printf("Error: %s\n", SDL_GetError());
			return 1;
		};
	}else { 
		for (int r = 0; r < map_params.resolution; ++r) {
			for (int c = 0; c < map_params.resolution; ++c) {
				printf("%3d", height_map[r * map_params.resolution + c]);
			};
			printf("\n");
		}
		printf("\n");

		height_t* display = map_trim(height_map, map_params, 16, 9);
		for (int r = 0; r < 9; ++r) {
			for (int c = 0; c < 16; ++c) {
				printf("%3d", display[(r * 16) + c]);
			};
			printf("\n");
		}
	};

	return 0;
}

