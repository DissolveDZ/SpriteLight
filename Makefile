
header:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/include/engine.h
	cp SpriteLight/include/engine.h include

dynamic: header
	clang SpriteLight/src/main.c -shared -ISpriteLight/engine_include -ISpriteLight/include -L. -lfreetype -fPIC -lSDL2main -lSDL2 -lm -o libSpriteLight.so -D_FILE_OFFSET_BITS=64 -ggdb
	clang src/main.c libSpriteLight.so -Iinclude -L. -lfreetype -lSDL2 -lSDL2_mixer -lm -ggdb -Wall -o engine_test

static: header
	clang -c SpriteLight/src/main.c -o libSpriteLight.o -ISpriteLight/include -lSDL2main -DSLDL_MAIN_HANDLED -lm -D_FILE_OFFSET_BITS=64 -ggdb
	ar rcs libSpriteLight.a libSpriteLight.o
	clang src/main.c libSpriteLight.o -Iinclude -L. -lfreetype -lSDL2 -lSDL2_mixer -lm -ggdb -Wall -o engine_test
	./engine_test

run: dynamic
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/SpriteLight ./engine_test