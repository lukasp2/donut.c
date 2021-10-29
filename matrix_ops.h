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

// calculates the dot product of v1 and v2 of size k
float dot_product(float** const v1, float** const v2, int k) {
    float result = 0.0;
    for (int i = 0; i < k; i++)
        result += v1[0][i] * v2[0][i];
    return result;
}

// rotates point p by r around x
float** rotate_x(float** p, float r) {
    float m[3][3] = {{1, 0,       0     },
		     {0, cos(r),  sin(r)},
		     {0, -sin(r), cos(r)}};
    return multiply(p, 1, 3, (float**)m, 3, 3);
}

// rotates point p by r around y
float** rotate_y(float** p, float r) {
    float m[3][3] = {{cos(r),  0, sin(r)},
		     {0,       1, 0     },
		     {-sin(r), 0, cos(r)}};
    return multiply(p, 1, 3, (float**)m, 3, 3);
}

// rotates point p by r around z
float** rotate_z(float** p, float r) {
    float m[3][3] = {{cos(r),  sin(r), 0},
		     {-sin(r), cos(r), 0},
		     {0,       0,      1}};
    return multiply(p, 1, 3, (float**)m, 3, 3);
}

// gets a normal vector for a point on the shape given u, v
float** get_normal_vector(float** shape, float u, float v) {
    float point[1][3] = {{cos(u), sin(u), 0}};
    float** normal_v = multiply((float**)point, 1, 3, shape, 3, 3);
    return normal_v;
}
