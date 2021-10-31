#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "matrix_ops.h"

int main() {
    Matrix* light_source;
    matrix_init(light_source, 1, 3);
    float light_source_arr[1][3] = {{0, 1, -1}}; // location of light source
    matrix_set(light_source, light_source_arr);

    float dist_screen = 4; // distance from observer to the screen
    float dist_shape = 5; // distance from observer to the shape
    float r1 = 1; // circle radius
    float r2 = 2; // circle offset from origin

    Matrix* torus;
    matrix_init(torus, 1, 3);
    
    Matrix* normal;
    matrix_init(normal, 1, 3);
    
    Matrix* rotation;
    matrix_init(rotation, 3, 3);

    // ** //
    float A = 0, B = 0;
    float z_buffer[1760];
    char frame_buffer[1760];

    float pi = 3.1415;
    
    printf("\x1b[2J");

    for (;;) {
        memset(frame_buffer, 32, 1760);
        memset(z_buffer, 0, 7040);
	
        for (float u = 0; u < 2*pi; u += 0.02) {
            for (float v = 0; v < 2*pi; v += 0.007) {
		// 1. describe the circle
		float arr[1][3] = {{r2 + r1 * cos(u), r1 * sin(u), 0}};
		matrix_set(torus, arr);
		
		// 2. rotate the circle around y by v
		matrix_set_rotation(rotation, y, v);
		matrix_multiply(torus, rotation, torus);
		
		// 3. rotate the torus around x by A and around z by B
		matrix_set_rotation(rotation, x, A);
		matrix_multiply(torus, rotation, torus);

		matrix_set_rotation(rotation, z, B);
		matrix_multiply(torus, rotation, torus);

		// 4. project shape to 2D plane
		matrix_project(torus, dist_screen, dist_shape);
		float x = torus->values[0][0];
		float y = torus->values[0][1];
		float z = torus->values[0][2];
		
		// 5. get luminance
		int lum = 8 * matrix_get_luminance(torus, light_source, normal, u, v);

		// 6. store z value of what we draw
		float D = 1 / z;

		int write_idx = x + 80 * y;
		
		// perimeter of view
                if (22 > y && y > 0 && 80 > x && x > 0 && D > z_buffer[write_idx]) {
                    z_buffer[write_idx] = D;
                    frame_buffer[write_idx] = ".,-~:;=!*#$@"[lum > 0 ? lum : 0];
                }
            }
        }

	printf("\x1b[H");

	// rotational movement
	for (int k = 0; k < 1761; k++)
	{
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
		
