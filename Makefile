header:
	./merge_headers.sh

dynamic: header
	meson compile -C build sprite_light_shared
	meson compile -C build engine_test_shared
	cp build/libsprite_light_shared.so .
	cp build/engine_test_shared .
	cp build/compile_commands.json .
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)
	./engine_test_shared

static: header
	meson compile -C build sprite_light_static
	meson compile -C build engine_test_static
	cp build/libsprite_light_static.a .
	cp build/engine_test_static .
	cp build/compile_commands.json .
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)
	./engine_test_static

run: dynamic static
