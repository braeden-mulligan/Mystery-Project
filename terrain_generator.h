
struct terrain_parameters {
	int resolution;
	int height;
	int noise;
	// enforce grade range of [-1, 1].
	float grade_x; 
	float grade_y;
	int smoothing;
};

void map_generate(int* height_map, struct terrain_parameters params);

//void map_trim();
