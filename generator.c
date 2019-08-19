#include <stdlib.h>
#include <stdio.h>

//TODO: do smoothing

void square(int** h_map, int row, int col, int chunk_size) {
	float sum = h_map[row - chunk_size / 2][col - chunk_size / 2];
	sum += h_map[row - chunk_size / 2][col + chunk_size / 2];
	sum += h_map[row + chunk_size / 2][col - chunk_size / 2];
	sum += h_map[row + chunk_size / 2][col + chunk_size / 2];
	float avg = sum / 4;

    h_map[row][col] = (int)avg; //TODO: + perturbance.
}

void diamond(int** h_map, int row, int col, int chunk_size, int map_size) {
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
	h_map[row][col] = (int)avg; //TODO: + perturbance.
}

void map_generate(int** h_map, int map_size) {
	int chunk_size = map_size - 1;
	while (chunk_size > 1) {

		// chunk_size should be power of 2.
		int half_chunk = chunk_size / 2; 

		for (int r = half_chunk; r < map_size; r += chunk_size) {
			for (int c = half_chunk; c < map_size; c += chunk_size) {
				square(h_map, r, c, chunk_size);
			}
		}

		for (int r = 0; r < map_size; r += half_chunk) {
			for (int c = (r + half_chunk) % chunk_size; c < map_size; c += chunk_size) {
				diamond(h_map, r, c, chunk_size, map_size);
			}
		}

		chunk_size /= 2;
	}
//TODO: trim map
}

int main() {

	//TODO: get from input
	int n = 33; 
	int* height_map[n];
	for (int i = 0; i < n; ++i) {
		height_map[i] = malloc(n * sizeof(int));
	}

	/*
	int** height_map = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		height_map[i] = malloc(n * sizeof(int));
	}
	*/

	//TODO: get from input
	height_map[0][0] = 20;
	height_map[0][n - 1] = 15;
	height_map[n - 1][0] = 10;
	height_map[n - 1][n - 1] = 5;
	map_generate(height_map, n);
	
	// Display map.
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%2d ", height_map[i][j]);
		};
		printf("\n");
	}
	return 0;
}

