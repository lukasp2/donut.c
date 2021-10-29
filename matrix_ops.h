#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// multiplies two matrixes m{1,2} (mxr rows, mxc columns)
float** multiply(float** const m1, int m1r, int m1c,
	       float** const m2, int m2r, int m2c) {
    float* values = calloc(m1r * m2c, sizeof(float));
    float** result = malloc(m1r * sizeof(float*));
    
    for (int rows = 0; rows < m1r; ++rows) {
	for (int cols = 0; cols < m2c; ++cols) {
	    int sum = 0;
	    for (int k = 0; k < m1r; ++k)
		sum += m1[rows][k] * m2[k][cols];
	    result[rows][cols] = sum;
	}
    }
    
    return result;
}

// rotates point p by r around x
void rotate_x(float** p, float r) {
    float m[3][3] = {{1, 0,       0     },
		     {0, cos(r),  sin(r)},
		     {0, -sin(r), cos(r)}};
    multiply(p, 1, 3, (float**)m, 3, 3);
}

// rotates point p by r around y
void y_rotation(float** p, float r) {
    float m[3][3] = {{1, 0,       0     },
		     {0, cos(r),  sin(r)},
		     {0, -sin(r), cos(r)}};
    multiply(p, 1, 3, (float**)m, 3, 3);
}

// rotates point p by r around z
void z_rotation(float** p, float r) {
    float m[3][3] = {{cos(r),  sin(r), 0},
		     {-sin(r), cos(r), 0},
		     {0,       0,      1}};
    multiply(p, 1, 3, (float**)m, 3, 3);
}

// gets light intensity for a point
float get_luminance(float u, float v) {
    // 1. set light source location
    int light_source[3] = {0, 1, -1};
    
    // 2. get normal vector of point
    int normal_v[] = {};
    
    // 3. get dot product of ls - nv
    float dot_product = 0;
    
    return dot_product;
}

// projects 3D point on 2D plane
void project(int* const xyz) {
    int k1 = 4; // z dist origin to screen
    int k2 = 10; // z dist origin to object
    xyz[0] = k1 * xyz[0] / (k2 + xyz[2]);
    xyz[1] = k1 * xyz[1] / (k2 + xyz[2]);
}
