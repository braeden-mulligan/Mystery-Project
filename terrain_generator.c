#include <stdlib.h>
#include <time.h>

#include "terrain_generator.h"

void init_terrain_parameters(struct terrain_parameters* params) {
	params->resolution = 2;
	params->height = 1;
	params->noise = 1;
	params->seed = 0;
	params->grade_x = 0.0; 
	params->grade_y = 0.0;
	params->smoothing = 0;
}

//TODO: do smoothing
//TODO: heigh limit
// 'limit' must be greater than 0.
height_t limit_height(int limit, double value) {
	return (height_t)value;
}

double probability() {
	return (double)rand() / (double)RAND_MAX;
}

void square(height_t* height_map, int row, int col, int chunk_size, int noise_scale, struct terrain_parameters* params) {

	height_t sum;
	sum = height_map[(row - chunk_size / 2) * params->resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params->resolution + (col + chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params->resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params->resolution + (col + chunk_size / 2)];
	double avg = (double)sum / 4.0;

	double height_point = avg + (probability() * 2.0 - 1.0) * noise_scale;
	height_map[(row * params->resolution) + col] = limit_height(params->height, height_point);
}

void diamond(height_t* height_map, int row, int col, int chunk_size, int noise_scale, struct terrain_parameters* params) {
	height_t sum = 0;
	int pts = 0;
	if (row - chunk_size / 2 >= 0) {
		sum += height_map[(row - chunk_size / 2) * params->resolution + col];
		pts++;
	};
	if (row + chunk_size / 2 < params->resolution) {
		sum += height_map[(row + chunk_size / 2) * params->resolution + col];
		pts++;
	};  
	if (col - chunk_size / 2 >= 0) {
		sum += height_map[(row * params->resolution) + (col - chunk_size / 2)];
		pts++;
	};  
	if (col + chunk_size / 2 < params->resolution) {
		sum += height_map[(row * params->resolution) + (col + chunk_size / 2)];
		pts++;
	};  
	double avg = (double)sum / (double)pts;

	double height_point = avg + (probability() * 2.0 - 1.0) * noise_scale;
	height_map[(row * params->resolution) + col] = limit_height(params->height, height_point);
}


height_t* map_trim(height_t* height_map, struct terrain_parameters params, int x, int y) {
	height_t* new_map = malloc(x * y * sizeof(height_t));

	for (int r = 0; r < y; ++r) {
		for (int c = 0; c < x; ++c) {
			new_map[(r * x) + c] = height_map[(r * params.resolution) + c];
		}
	}

	return new_map;
}

void map_generate(height_t* height_map, struct terrain_parameters params, int dim_x, int dim_y) {
	if (params.seed) {
		srand(params.seed);
	}else {
		srand(time(0));
	};

	if (dim_x && dim_y) {
		//TODO: change resolution to cover dimensions max(x, y), then trim.
	};

	int chunk_size = params.resolution - 1;
	double noise_scale = (double)params.noise;

	//TODO: get from parameters
	height_map[0] = 10;
	height_map[chunk_size] = 15;
	height_map[chunk_size * params.resolution] = 5;
	height_map[(chunk_size * params.resolution) + chunk_size] = 10;

	while (chunk_size > 1) {
		// chunk_size should be power of 2.
		int half_chunk = chunk_size / 2; 

		for (int r = half_chunk; r < params.resolution; r += chunk_size) {
			for (int c = half_chunk; c < params.resolution; c += chunk_size) {
				square(height_map, r, c, chunk_size, noise_scale, &params);
			}
		}

		for (int r = 0; r < params.resolution; r += half_chunk) {
			for (int c = (r + half_chunk) % chunk_size; c < params.resolution; c += chunk_size) {
				diamond(height_map, r, c, chunk_size, noise_scale, &params);
			}
		}

		chunk_size /= 2;
		noise_scale /= 2.0;
	}

	if (dim_x && dim_y) {
		height_t* trimmed_map = map_trim(height_map, params, dim_x, dim_y);
		free(height_map);
		height_map = trimmed_map;
	};
}


