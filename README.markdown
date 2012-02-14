Mandelbrot set viewer with a focus on speed. **Zoom and explore in real time.**

#### Controls:
        Left click      zoom in
        Right click     zoom out
        Arrow keys      move around
        q               quit
        +               increase iterations
        -               decrease iterations

#### Features:
*   Uses the continuous coloring algorithm for sexy smooth renderings.
*   Adjust the number of iterations to change the coloring as desired.
*   Zoom in more than 1,000,000,000,000x original scale (approximately).

Standalone binaries are available for OS X and Linux in the Downloads section.

To build it yourself, you need SDL and a C99 compliant compiler that supports
nested functions (GCC works).

#### Known bugs:
*   Produces inaccurate renderings at very specific places due to the nature of
    one of the optimizations; see if you can find them!
