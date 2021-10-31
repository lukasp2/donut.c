
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

enum Axis {x, y, z};

struct Matrix {
    int r;
    int c;
    float **values;
};

typedef float Radian;
typedef enum Axis Axis;
typedef struct Matrix Matrix;

void matrix_init(Matrix* m, int r, int c) {
    m->r = r;
    m->c = c;
    m->values = malloc(sizeof(float[r][c]));
    assert(m->values != NULL);
}

void matrix_set(Matrix* m, float values[m->r][m->c]) {
    for (int i = 0; i < m->r; ++i) {
        for (int ii = 0; ii < m->c; ++ii) {
	    m->values[i][ii] = values[i][ii];
	}
    }
}

void matrix_free(Matrix* m) {
    free(m->values);
}

void matrix_set_rotation(Matrix* m, Axis axis, Radian r) {
    if ( axis == x ) {
	float rot[3][3] = {{1, 0,       0     },
			   {0, cos(r),  sin(r)},
			   {0, -sin(r), cos(r)}};
	matrix_set(m, rot);
    }
    else if ( axis == y ) {
    	float rot[3][3] = {{cos(r),  0, sin(r)},
			   {0,       1, 0     },
			   {-sin(r), 0, cos(r)}};
	matrix_set(m, rot);
    }
    else {
    	float rot[3][3] = {{cos(r),  sin(r), 0},
			   {-sin(r), cos(r), 0},
			   {0,       0,      1}};
	matrix_set(m, rot);
    }
}

// multiplies two matrixes m{1,2}
void matrix_multiply(const Matrix* const m1, const Matrix* const m2, Matrix* result) {
    assert(m1->c == m2->r);
    
    for (int r = 0; r < m1->r; ++r) {
	for (int c = 0; c < m2->c; ++c) {
	    int sum = 0;
	    for (int k = 0; k < m1->c; ++k)
		sum += m1->values[r][k] * m2->values[k][c];
	    result->values[r][c] = sum;
	}
    }

}
// calculates the dot product of v1 and v2 of size k
float matrix_get_dot_product(const Matrix* const m1, const Matrix* const m2) {
    assert(m1->c == m2->c);
    
    float result = 0.0;
    for (int i = 0; i < m1->c; i++)
        result += m1->values[0][i] * m2->values[0][i];
    return result;
}

// gets a normal vector for a point on the shape given u, v
void matrix_set_normal(const Matrix* const m, float u, float v, Matrix* normal) {
    Matrix* m2;
    matrix_init(m2, 1, 3);
    
    float f[1][3] = {{cos(u), sin(u), 0}};
    matrix_set(m2, f);

    matrix_multiply(m2, m, normal);

    matrix_free(m2);
}

// projects 3D point on 2D plane
void matrix_project(const Matrix* const m, float dist_screen, float dist_shape) {
    m->values[0][0] = dist_screen * m->values[0][0] / (dist_shape + m->values[0][2]);
    m->values[0][1] = dist_screen * m->values[0][1] / (dist_shape + m->values[0][2]);
}

// gets light intensity for a point
float matrix_get_luminance(const Matrix* const m,
			   const Matrix* const light_source,
			   Matrix* const normal, float u, float v) {
    matrix_set_normal(m, u, v, normal);
    return matrix_get_dot_product(light_source, normal);
}

