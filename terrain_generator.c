#include <stdlib.h>
#include <time.h>

#include "terrain_generator.h"

//TODO: do smoothing

void square(int* height_map, int row, int col, int chunk_size, int resolution, int noise) {

	float sum = height_map[(row - chunk_size / 2) * resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * resolution + (col + chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * resolution + (col - chunk_size / 2)];
	sum += height_map[(row + chunk_size / 2) * resolution + (col + chunk_size / 2)];
	float avg = sum / 4;

	height_map[(row * resolution) + col] = ((int) avg) + ((rand() - (RAND_MAX / 2)) % noise);
}

void diamond(int* height_map, int row, int col, int chunk_size, int resolution, int noise) {
	int sum = 0;
	int pts = 0;

	if (row - chunk_size / 2 >= 0) {
		sum += height_map[(row - chunk_size / 2) * resolution + col];
		pts++;
	};
	if (row + chunk_size / 2 < resolution) {
		sum += height_map[(row + chunk_size / 2) * resolution + col];
		pts++;
	};  
	if (col - chunk_size / 2 >= 0) {
		sum += height_map[(row * resolution) + (col - chunk_size / 2)];
		pts++;
	};  
	if (col + chunk_size / 2 < resolution) {
		sum += height_map[(row * resolution) + (col + chunk_size / 2)];
		pts++;
	};  
	
	float avg = sum / pts;
	height_map[(row * resolution) + col] = ((int) avg) + ((rand() - (RAND_MAX / 2)) % noise);
}

void map_generate(int* height_map, struct terrain_parameters params) {
	srand(time(0));

	int chunk_size = params.resolution - 1;
	int noise_scale = params.noise;

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
				square(height_map, r, c, chunk_size, params.resolution, noise_scale);
			}
		}

		for (int r = 0; r < params.resolution; r += half_chunk) {
			for (int c = (r + half_chunk) % chunk_size; c < params.resolution; c += chunk_size) {
				diamond(height_map, r, c, chunk_size, params.resolution, noise_scale);
			}
		}

		chunk_size /= 2;
		if (noise_scale > 1) noise_scale /= 2;
	}
}

