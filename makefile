BUILD = gcc -Wall -ansi -std=c99 -L/opt/local/lib `sdl-config --cflags --static-libs`
mac:
	$(BUILD) -o mandelbrot_mac *.c -O3 -fnested-functions

debug-mac:
	$(BUILD) -o mandelbrot_debug_mac *.c -fnested-functions -pg

linux:
	$(BUILD) -o mandelbrot_linux *.c -O3

debug-linux:
	$(BUILD) -o mandelbrot_debug_linux *.c -pg
