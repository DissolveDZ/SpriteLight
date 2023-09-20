
header:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/include/engine.h
	cp SpriteLight/include/engine.h include

dynamic: header
	gcc SpriteLight/src/main.c -shared -ISpriteLight/engine_include -ISpriteLight/include -L. -lfreetype -fPIC -lSDL2main -lSDL2 -lm -o libSpriteLight.dll -D_FILE_OFFSET_BITS=64 -ggdb
	gcc src/main.c libSpriteLight.dll -o engine_test -Iinclude -L. -lfreetype -lSDL2 -lSDL2_mixer -lm -ggdb -Wall
	
static: header
	gcc -c SpriteLight/src/main.c -o libSpriteLight.o -ISpriteLight/include -lSDL2main -DSLDL_MAIN_HANDLED -lm -D_FILE_OFFSET_BITS=64 -ggdb
	ar rcs libSpriteLight.a libSpriteLight.o
	gcc -static-libgcc src/main.c libSpriteLight.o -Iinclude -L. -lfreetype -lSDL2 -lSDL2_mixer -lm -ggdb -Wall -o engine_test
	./engine_test

run: dynamic
	./engine_test