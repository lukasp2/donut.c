#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "matrix_ops.h"

int main() {
    float dist_screen = 4; // distance from observer to the screen
    float dist_shape = 5; // distance from observer to the shape
    float r1 = 1; // circle radius
    float r2 = 2; // circle offset from origin

    Matrix* torus = matrix_init(1, 3);
    Matrix* normal = matrix_init(1, 3);

    Matrix* rotation = matrix_init(3, 3);
    float rotation_arr[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    matrix_set(rotation, rotation_arr);
    
    Matrix* light_source = matrix_init(1, 3);
    float light_source_arr[1][3] = {{0, 1, -1}}; // location of light source
    matrix_set(light_source, light_source_arr);
    
    float A = 0, B = 0;
    float z_buffer[1760];
    char frame_buffer[1760];

    const float PI = 3.1415;
    
    printf("\x1b[2J");

    for (;;) {
        memset(frame_buffer, 32, 1760);
        memset(z_buffer, 0, 7040);
	
        for (float u = 0; u < 2*PI; u += 0.02) {
            for (float v = 0; v < 2*PI; v += 0.007) {
		// 1. describe the circle
		float torus_arr[1][3] = {{r2 + r1 * cos(u), r1 * sin(u), 0}};
		matrix_set(torus, torus_arr);
		
		// 2. setup rotation matrix to rotate around y by v
		matrix_rotate(rotation, y, v);
		
		// 3. rotate the torus around x by A and around z by B
		matrix_rotate(rotation, x, A);
		matrix_rotate(rotation, z, B);

		// 4. apply rotation on torus
		matrix_multiply(torus, rotation, torus);

		// 5. get luminance
		matrix_set_normal(rotation, u, v, normal);
		int lum = 8 * matrix_get_luminance(rotation, normal, light_source, u, v);
		
		// 6. project shape to 2D plane
		matrix_project(torus, dist_screen, dist_shape);
		float x = torus->values[0][0];
		float y = torus->values[0][1];
		float z = torus->values[0][2];

		// 7. store z value of what we draw
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
		
