all:
	gcc src/*.c -I./include -I/usr/include/SDL -o FBoD -lSDL -lGL -lGLU

clean:
	rm -f FBoD
