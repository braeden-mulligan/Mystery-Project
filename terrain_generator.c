#include <stdlib.h>
#include <time.h>

#include "terrain_generator.h"

	#include <stdio.h>

void init_terrain_parameters(struct terrain_parameters* params_p) {
	params_p->resolution = 2;
	params_p->height = 1;
	params_p->noise = 1;
	params_p->seed = 0;
	params_p->grade_x = 0.0; 
	params_p->grade_y = 0.0;
	params_p->smoothing = 0;
}

//TODO: do smoothing
//TODO: heigh limit

// 'limit' must be greater than 0.
height_t limit_height(double value, int limit) {
	return (height_t)value;
}

double probability() {
	return (double)rand() / (double)RAND_MAX;
}

void square(height_t* height_map, struct terrain_parameters* params_p, int row, int col, int chunk_size, int noise_scale) {

	height_t sum;
	sum = height_map[(row - chunk_size / 2) * params_p->resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params_p->resolution + (col + chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params_p->resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * params_p->resolution + (col + chunk_size / 2)];
	double avg = (double)sum / 4.0;

	double height_point = avg + (probability() * 2.0 - 1.0) * noise_scale;
	height_map[(row * params_p->resolution) + col] = limit_height(height_point, params_p->height);
}

void diamond(height_t* height_map, struct terrain_parameters* params_p, int row, int col, int chunk_size, int noise_scale) {
	height_t sum = 0;
	int pts = 0;
	if (row - chunk_size / 2 >= 0) {
		sum += height_map[(row - chunk_size / 2) * params_p->resolution + col];
		pts++;
	};
	if (row + chunk_size / 2 < params_p->resolution) {
		sum += height_map[(row + chunk_size / 2) * params_p->resolution + col];
		pts++;
	};  
	if (col - chunk_size / 2 >= 0) {
		sum += height_map[(row * params_p->resolution) + (col - chunk_size / 2)];
		pts++;
	};  
	if (col + chunk_size / 2 < params_p->resolution) {
		sum += height_map[(row * params_p->resolution) + (col + chunk_size / 2)];
		pts++;
	};  
	double avg = (double)sum / (double)pts;

	double height_point = avg + (probability() * 2.0 - 1.0) * noise_scale;
	height_map[(row * params_p->resolution) + col] = limit_height(height_point, params_p->height);
}

void terrain_generate(height_t* height_map, struct terrain_parameters* params_p) {
	if (params_p->seed) {
		srand(params_p->seed);
	}else {
		srand(time(0));
	};

	int chunk_size = params_p->resolution - 1;
	double noise_scale = (double)params_p->noise;

	//TODO: find good way to initialize.
	float elevation = ((float)params_p->height / 2.0) * probability(); 
	float m_x = params_p->grade_x;
	float m_y = params_p->grade_y;
	float h = (float)params_p->height / 2.0;
	height_map[0] = (height_t)(elevation + ((-(m_x+ m_y) / 2.0) * h));
	height_map[chunk_size] = (height_t)(elevation + (((m_x - m_y) / 2.0) * h));
	height_map[chunk_size * params_p->resolution] = (height_t)(elevation + (((-m_x + m_y) / 2.0) * h));
	height_map[(chunk_size * params_p->resolution) + chunk_size] = (height_t)(elevation + (((m_x + m_y) / 2.0) * h));

	while (chunk_size > 1) {
		// chunk_size should be power of 2.
		int half_chunk = chunk_size / 2; 

		for (int r = half_chunk; r < params_p->resolution; r += chunk_size) {
			for (int c = half_chunk; c < params_p->resolution; c += chunk_size) {
				square(height_map, params_p, r, c, chunk_size, noise_scale);
			}
		}

		for (int r = 0; r < params_p->resolution; r += half_chunk) {
			for (int c = (r + half_chunk) % chunk_size; c < params_p->resolution; c += chunk_size) {
				diamond(height_map, params_p, r, c, chunk_size, noise_scale);
			}
		}

		chunk_size /= 2;
		noise_scale /= 2.0;
	}
}

height_t* map_trim(height_t* height_map, struct terrain_parameters* params_p, int x, int y) {
	height_t* new_map = malloc(x * y * sizeof(height_t));

	// Trim border to sample from the center of original map.
	int offset_x = (params_p->resolution - x) / 2;
	int offset_y = (params_p->resolution - y) / 2;

	for (int r = 0; r < y; ++r) {
		for (int c = 0; c < x; ++c) {
			new_map[(r * x) + c] = height_map[((r + offset_y) * params_p->resolution) + c + offset_x];
		}
	}

	return new_map;
}

// Scale map down to 1/'scale by changing tile sizes to 'scale'.
//TODO: Debug this
void map_rescale(height_t* height_map, struct terrain_parameters* params_p, int x, int y, int scale) {
	for (int r = 0; r < y; r += scale) {
		for (int c = 0; c < x; c += scale) {
			printf("%3d", height_map[r * x + c]);

			height_t sample;
			/*
			for (int r_tile = r; r_tile < (r + scale); ++r_tile) {
				for (int c_tile = c; c_tile < (c + scale); ++c_tile) {
					sample = height_map[(r_tile * x) + c_tile];
				}
			}
			sample = height_map[(r * x) + c];
			for (int r_tile = r; r_tile < (r + scale); ++r_tile) {
				for (int c_tile = c; c_tile < (c + scale); ++c_tile) {
					height_map[(r_tile * x) + c_tile] = sample;
				}
			}
			*/

		}
		printf("\n");

	}
	printf("\n");
}

// wrapper for generate and modify functions.
void map_create(height_t* height_map, struct terrain_parameters params, int x, int y, int scale) {
/*
	if (x && y) {
		//TODO: get resolution automatically to cover dimensions max(x, y), then trim.
		// Calculate necessary resolution.
	};
*/
	terrain_generate(height_map, &params);

	if (x && y) {
		height_t* trimmed_map = map_trim(height_map, &params, x, y);
		free(height_map);
		height_map = trimmed_map;
	};

//TODO: debug
	for (int r = 0; r < params.resolution; ++r) {
		for (int c = 0; c < params.resolution; ++c) {
			printf("%3d", height_map[r * params.resolution + c]);
		};
		printf("\n");
	}
	printf("\n");
	if (scale) map_rescale(height_map, &params, x, y, scale);
}

