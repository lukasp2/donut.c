#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "matrix_ops.h"

float LIGHT_SOURCE[1][3] = {{0, 1, -1}}; // location of light source
float DIST_SCREEN = 4; // distance from observer to the screen
float DIST_SHAPE = 5; // distance from observer to the shape
float R1 = 1; // circle radius
float R2 = 2; // circle offset from origin

// gets light intensity for a point
float get_luminance(float** const shape, float u, float v) {
    float** normal = get_normal_vector(shape, u, v);
    return dot_product((float**)LIGHT_SOURCE, normal, 3);
}

// projects 3D point on 2D plane
void project(float** const p) {
    p[0][0] = DIST_SCREEN * p[0][0] / (DIST_SHAPE + p[0][2]);
    p[0][1] = DIST_SCREEN * p[0][1] / (DIST_SHAPE + p[0][2]);
    p[0][2] = DIST_SHAPE;
}

int main() {
    float A = 0, B = 0;
    float z[1760];
    char b[1760];

    float pi = 3.1415;
    
    printf("\x1b[2J");

    for (;;) {
        memset(b, 32, 1760);
        memset(z, 0, 7040);

        for (float u = 0; u < 2*pi; u += 0.02) {
            for (float v = 0; v < 2*pi; v += 0.007) {
		// 1. describe the circle
		float circle[1][3] = {{R2 + R1 * cos(u), R1 * sin(u), 0}};
		
		// 2. rotate the circle around y by v
		float** torus = rotate_y((float**)circle, v);

		// 3. rotate the torus around x by A and around z by B
		torus = rotate_x(torus, A);
		torus = rotate_z(torus, B);

		// 4. project shape to 2D plane
		project(torus);
		float x = torus[0][0];
		float y = torus[0][1];
		
		// 5. get luminance
		int lum = 8 * get_luminance(torus, u, v);

		// 6. store z value of what we draw
		float D = 1 / torus[0][2];

		int write_idx = x + 80 * y;
		
		// perimeter of view
                if (22 > y && y > 0 && 80 > x && x > 0 && D > z[write_idx]) {
                    z[write_idx] = D;
                    b[write_idx] = ".,-~:;=!*#$@"[lum > 0 ? lum : 0];
                }
            }
        }

	printf("\x1b[H");

	// rotational movement
	for (int k = 0; k < 1761; k++)
	{
            putchar(k % 80 ? b[k] : 10);
            A += 0.00002;
            B += 0.00001;
        }
	
        usleep(30000);
    }
    
    return 0;
}
		
