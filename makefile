all:
	gcc -o mandelbrot *.c -L/opt/local/lib -lSDL -lSDLmain -framework cocoa -O3 -fnested-functions

debug:
	gcc -o mandelbrot_debug *.c -L/opt/local/lib -lSDL -lSDLmain -framework cocoa -fnested-functions -pg
