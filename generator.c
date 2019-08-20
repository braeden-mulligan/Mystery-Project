#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//TODO: do smoothing

typedef struct map_params {
	int map_size;
	int amplitude;
	int smoothing;
} map_container_t;

void square(int** h_map, int row, int col, int chunk_size, int amp_scale) {
	float sum = h_map[row - chunk_size / 2][col - chunk_size / 2];
	sum += h_map[row - chunk_size / 2][col + chunk_size / 2];
	sum += h_map[row + chunk_size / 2][col - chunk_size / 2];
	sum += h_map[row + chunk_size / 2][col + chunk_size / 2];
	float avg = sum / 4;

	h_map[row][col] = ((int) avg) + ((rand() - (RAND_MAX / 2)) % amp_scale);
}

void diamond(int** h_map, int row, int col, int chunk_size, int map_size, int amp_scale) {
	int sum = 0;
	int pts = 0;

	if (row - chunk_size / 2 >= 0) {
		sum += h_map[row - chunk_size / 2][col];
		pts++;
	};
	if (row + chunk_size / 2 < map_size) {
		sum += h_map[row + chunk_size / 2][col];
		pts++;
	};  
	if (col - chunk_size / 2 >= 0) {
		sum += h_map[row][col - chunk_size / 2];
		pts++;
	};  
	if (col + chunk_size / 2 < map_size) {
		sum += h_map[row][col + chunk_size / 2];
		pts++;
	};  
	
	float avg = sum / pts;
	h_map[row][col] = ((int) avg) + ((rand() - (RAND_MAX / 2)) % amp_scale);
}

// amplitude determines how tall/jaggen map is.
void map_generate(int** h_map, map_container_t map_params) {
	srand(time(0));

	int chunk_size = map_params.map_size - 1;
	int amp_scale = map_params.amplitude;
	while (chunk_size > 1) {

		// chunk_size should be power of 2.
		int half_chunk = chunk_size / 2; 

		for (int r = half_chunk; r < map_params.map_size; r += chunk_size) {
			for (int c = half_chunk; c < map_params.map_size; c += chunk_size) {
				square(h_map, r, c, chunk_size, amp_scale);
			}
		}

		for (int r = 0; r < map_params.map_size; r += half_chunk) {
			for (int c = (r + half_chunk) % chunk_size; c < map_params.map_size; c += chunk_size) {
				diamond(h_map, r, c, chunk_size, map_params.map_size, amp_scale);
			}
		}

		chunk_size /= 2;
		amp_scale /= 2;
	}
//TODO: trim map
}

int main(int argc, char* argv[]) {
	// TODO: getopt()
	// Check to get parameters from arguments or from window objects.
	bool gui = false;
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-g")) gui = true; 
	}

	int dimension = 2;
	int height = 0;
	int noise = 0;
	if (gui) {
		//TODO: get params from window
	}else {
		// Debug
		for (int i = 1; i < argc; ++i) {
			if (!strcmp(argv[i], "-d")) dimension = atoi(argv[i + 1]);
			if (!strcmp(argv[i], "-h")) height = atoi(argv[i + 1]); 
			if (!strcmp(argv[i], "-n")) noise = atoi(argv[i + 1]); 
		}
	};
	map_container_t map_params = {.map_size = dimension,
		.amplitude = height,
		.smoothing = noise };

	int** height_map = malloc(dimension * sizeof(int*));
	for (int i = 0; i < dimension; ++i) {
		height_map[i] = malloc(dimension * sizeof(int));
	}
	//TODO: get from input
	height_map[0][0] = 20;
	height_map[0][dimension - 1] = 15;
	height_map[dimension  - 1][0] = 10;
	height_map[dimension - 1][dimension - 1] = 5;

	map_generate(height_map, map_params);
	
	// Display map.
	if (gui) {
		//SDL2
	else{ 
		for (int i = 0; i < dimension; ++i) {
			for (int j = 0; j < dimension; ++j) {
				printf("%3d", height_map[i][j]);
			};
			printf("\n");
		}
	};

	return 0;
}

