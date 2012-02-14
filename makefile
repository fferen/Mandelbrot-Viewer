mac:
	gcc -o mandelbrot_mac *.c `sdl-config --cflags --static-libs` -O3 -fnested-functions

debug-mac:
	gcc -o mandelbrot_debug_mac *.c `sdl-config --cflags --static-libs` -fnested-functions -pg

linux:
	gcc -o mandelbrot_linux *.c `sdl-config --cflags --static-libs` -O3

debug-linux:
	gcc -o mandelbrot_debug_linux *.c `sdl-config --cflags --static-libs` -pg
