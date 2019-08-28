typedef int height_t;

struct terrain_parameters {
	unsigned int seed;
	int resolution;
	int height;
	int noise;
	// enforce grade range of [-1, 1].
	float grade_x; 
	float grade_y;
	int smoothing;
};

void init_terrain_parameters(struct terrain_parameters* params);

height_t* map_create(struct terrain_parameters params, int x, int y, int scale);


