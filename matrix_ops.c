#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix_ops.h"

Matrix* matrix_init(int const r, int const c) {
    Matrix* m = malloc(sizeof(Matrix));
    assert(m != NULL);
    *(int *)&m->r = r;
    *(int *)&m->c = c;
    m->values = malloc(r * sizeof(float));
    assert(m->values != NULL);
    
    for (int i = 0; i < r; ++i) {
	m->values[i] = malloc(c * sizeof(float));
    }
    
    return m;
}

void matrix_rotate(Matrix* m, Axis const axis, Radian const r) {
    Matrix* rotation_matrix = matrix_init(3, 3);
    if ( axis == x ) {
	float rotation_array[3][3] = {{1, 0,       0     },
				      {0, cos(r),  sin(r)},
				      {0, -sin(r), cos(r)}};
	matrix_set(rotation_matrix, rotation_array);
    }
    if ( axis == y ) {
    	float rotation_array[3][3] = {{cos(r),  0, sin(r)},
				      {0,       1, 0     },
				      {-sin(r), 0, cos(r)}};
	matrix_set(rotation_matrix, rotation_array);
    }
    if (axis == z) {
    	float rotation_array[3][3] = {{cos(r),  sin(r), 0},
				      {-sin(r), cos(r), 0},
				      {0,       0,      1}};
	matrix_set(rotation_matrix, rotation_array);
    }
    matrix_multiply(m, rotation_matrix, m);
    matrix_free(rotation_matrix);
}

void matrix_set(Matrix* m, float const values[m->r][m->c]) {
    for (int i = 0; i < m->r; ++i) {
        for (int ii = 0; ii < m->c; ++ii) {
	    m->values[i][ii] = values[i][ii];
	}
    }
}

void matrix_free(Matrix* m) {
    for (int i = 0; i < m->r; i++) free(m->values[i]);
    free(m->values);
    free(m);
}

void matrix_set_rotation(Matrix* m, Axis const axis, Radian const r) {
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

void matrix_multiply(const Matrix* const m1, const Matrix* const m2, Matrix* const result) {
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

float matrix_get_dot_product(const Matrix* const m1, const Matrix* const m2) {
    assert(m1->c == m2->c);
    float result = 0.0;
    for (int i = 0; i < m1->c; i++)
        result += m1->values[0][i] * m2->values[0][i];
    return result;
}

void matrix_project(const Matrix* const m,
		    const float dist_screen,
		    const float dist_shape) {
    m->values[0][0] = dist_screen * m->values[0][0] / (dist_shape + m->values[0][2]);
    m->values[0][1] = dist_screen * m->values[0][1] / (dist_shape + m->values[0][2]);
}

void matrix_set_normal(const Matrix* const m,
		       const float u,
		       const float v,
		       Matrix* const normal) {
    Matrix* m2 = matrix_init(1, 3);
    float f[1][3] = {{cos(u), sin(u), 0}};
    matrix_set(m2, f);
    matrix_multiply(m2, m, normal);
    matrix_free(m2);
}

float matrix_get_luminance(Matrix* const m,
			   Matrix* const normal,
			   const Matrix* const light_source,
			   const float u,
			   const float v) {
    matrix_set_normal(m, u, v, normal);
    return matrix_get_dot_product(normal, light_source);
}

// fprintf(stderr, "point:\n"); matrix_print(m2);
void matrix_print(const Matrix* const m) {
    fprintf(stderr, "[%d][%d]", m->r, m->c);
    for (int r = 0; r < m->r; ++r) {
	fprintf(stderr, "\t");
	for (int c = 0; c < m->c; ++c) {
	    fprintf(stderr, "%f ", m->values[r][c]);
	}
	fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}
