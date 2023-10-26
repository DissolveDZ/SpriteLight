header:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/include/engine.h
	cp SpriteLight/include/engine.h include

dynamic: header
	clang SpriteLight/src/main.c -shared -ISpriteLight/engine_include -o libSpriteLight.so -ISpriteLight/include -L. -lfreetype -lflecs -fPIC -lSDL2 -D_FILE_OFFSET_BITS=64 -ggdb
	clang src/main.c libSpriteLight.so -Iinclude -L. -lfreetype -lflecs -lSDL3 -ggdb -o engine_test

static: header
	clang -c SpriteLight/src/main.c -o libSpriteLight.o -ISpriteLight/include -DSLDL_MAIN_HANDLED -lflecs -D_FILE_OFFSET_BITS=64 -ggdb
	ar rcs libSpriteLight.a libSpriteLight.o
	clang src/main.c libSpriteLight.o -Iinclude -L. -lfreetype -lflecs -lSDL3 -lm -ggdb -o engine_test
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:pwd
	./engine_test

run: dynamic
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:pwd
	./engine_test

# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(CURDIR) ./engine_test