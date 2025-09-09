
cflags = -Wall -Werror -pedantic -std=c99 -O2
dflags = -Wall -Werror -pedantic -std=c99 -g

sdl_include = deps/SDL3-3.2.20/i686-w64-mingw32/include
sdl_lib = deps/SDL3-3.2.20/i686-w64-mingw32/lib -lSDL3 -mwindows

source = $(wildcard src/*.c) $(wildcard src/**/*.c)
output = bin/render_cpu
DVAR = 
build:
	gcc $(cflags) $(DVAR) $(source) -o $(output) -I $(sdl_include) -L $(sdl_lib)
	copy "deps\\SDL3-3.2.20\\i686-w64-mingw32\\bin\\SDL3.dll" "bin\\SDL3.dll"
	@echo "Compilo exitosamente!!"


