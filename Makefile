CRUFT = *.o
FLAGS = -std=c99
FLAGS_compile_sdl2 = -I/usr/include/SDL2 -D_REENTRANT
FLAGS_link_sdl2 = -std=c99 -L/usr/lib/x86_64-linux-gnu -lSDL2

all: clean main

main: graphics terrain-generator
	gcc $(FLAGS) $(FLAGS_link_sdl2) -o mystery-project graphics.o terrain_generator.o

graphics:
	gcc $(FLAGS) $(FLAGS_compile_sdl2) -c graphics.c -o graphics.o

terrain-generator:
	gcc $(FLAGS) -c terrain_generator.c -o terrain_generator.o

clean:
	rm -f $(CRUFT)

