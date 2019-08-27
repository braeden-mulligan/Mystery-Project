typedef int height_t;

struct terrain_parameters {
	int resolution;
	int height;
	int noise;
	unsigned int seed;
	// enforce grade range of [-1, 1].
	float grade_x; 
	float grade_y;
	int smoothing;
};

void init_terrain_parameters(struct terrain_parameters* params);

void map_generate(height_t* height_map, struct terrain_parameters params, int x_dim, int y_dim);

height_t* map_trim(height_t* height_map, struct terrain_parameters params, int x, int y);

