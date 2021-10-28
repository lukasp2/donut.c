#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

int main() {
    float A = 0, B = 0;
    float z[1760];
    char b[1760];

    float pi = 3.1415;
    
    printf("\x1b[2J");

    for (;;) {
        memset(b, 32, 1760);
        memset(z, 0, 7040);

        for (float u = 0; u < 1; u += 0.02) {
            for (float v = 0; v < 1; v += 0.007) {
		
                float f = sin(2*pi*u); // cicle y (f)
                float d = cos(2*pi*u); // cicle x (d)

                float c = sin(2*pi*v); // torus y (c)
                float l = cos(2*pi*v); // torus x (l)

                float R = d + 2; // torus radius

                float e = sin(A); // 
                float g = cos(A); // 
                float n = sin(B); // 
		float m = cos(B); // 

                float D = 1 / (c * R * e + f * g + 5); // view distance

                float t = c * R * g - f * e; // some alias prob

		// project 3D point on 2D (x, y) plane
                int x = 40 + 30 * D * (l * R * m - t * n);
                int y = 12 + 15 * D * (l * R * n + t * m);

		int write_idx = x + 80 * y;
		
		 // perimeter of view
                if (22 > y && y > 0 && 80 > x && x > 0 && D > z[write_idx]) {
		    // normal vector (for lighting)
		    int light_vector = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);

                    z[write_idx] = D;
                    b[write_idx] = ".,-~:;=!*#$@"[light_vector > 0 ? light_vector : 0];
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
