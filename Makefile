CRUFT = *.o
OPTS = -std=c99

all: clean generator

generator:
	gcc $(OPTS) -c generator.c -o generator.o
	gcc $(OPTS) -o terrain_generator generator.o

clean:
	rm -f $(CRUFT)
