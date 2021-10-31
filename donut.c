#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "matrix_ops.h"

int main() {
    double dist_screen = 4; // distance from observer to the screen
    double dist_shape = 5; // distance from observer to the shape
    double r1 = 1; // circle radius
    double r2 = 2; // circle offset from origin

    Matrix* torus = matrix_init(1, 3);
    Matrix* normal = matrix_init(1, 3);

     // set location of light source to (0, 1, -1)
    Matrix* light_source = matrix_init(1, 3);
    double light_source_arr[1][3] = {{0, 1, -1}};
    matrix_set(light_source, light_source_arr);

    // rotation matrix
    Matrix* rotation = matrix_init(3, 3);
    double rotation_arr[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    double A, B = 0;
    double z_buffer[1760];
    char frame_buffer[1760];
    
    printf("\x1b[2J");   
    for (;;) {
        memset(frame_buffer, 32, 1760);
        memset(z_buffer, 0, 7040);

        for (double u = 0; u < 2*M_PI; u += 0.04) { // 0.04
            for (double v = 0; v < 2*M_PI; v += 0.07) { // 0.07
		// 1. describe a circle
		double torus_arr[1][3] = {{r2 + r1 * cos(v), r1 * sin(v), 0}};
		matrix_set(torus, torus_arr);

		// 2. setup rotation matrix
		matrix_set(rotation, rotation_arr);
		matrix_rotate(rotation, y, u);
		matrix_rotate(rotation, x, A);
		matrix_rotate(rotation, z, B);
		
		// 2. apply rotation into a spinning torus
		matrix_multiply(torus, rotation, torus);

		// 4. calculate luminance
		int lum = 8 * matrix_get_luminance(rotation, normal, light_source, u, v);
		fprintf(stderr, "lum = %d\n", lum);

		// 5. project shape to 2D plane
		matrix_project(torus, dist_screen, dist_shape);
		double x = torus->values[0][0];
		double y = torus->values[0][1];

		// 6. store z value of what we draw
		double z = 1 / torus->values[0][2];

		int write_idx = x + 80 * y;		
                if (22 > y && y > 0 && 80 > x && x > 0 && z > z_buffer[write_idx]) {
                    z_buffer[write_idx] = z;
                    frame_buffer[write_idx] = ".,-~:;=!*#$@"[lum > 0 ? lum : 0];
                }
            }
        }
	
	printf("\x1b[H");

	for (int k = 0; k < 1761; k++) {
            putchar(k % 80 ? frame_buffer[k] : 10);
            A += 0.00002;
            B += 0.00001;
	}
	
        usleep(30000);
    }
    
    matrix_free(torus);
    matrix_free(normal);
    matrix_free(rotation);
    matrix_free(light_source);
    
    return 0;
}
		
