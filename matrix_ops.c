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
    m->values = malloc(r * sizeof(double));
    assert(m->values != NULL);
    
    for (int i = 0; i < r; ++i) {
	m->values[i] = malloc(c * sizeof(double));
    }
    
    return m;
}

void matrix_free(Matrix* m) {
    for (int i = 0; i < m->r; i++) free(m->values[i]);
    free(m->values);
    free(m);
}

void matrix_set(Matrix* m, double const values[m->r][m->c]) {
    for (int i = 0; i < m->r; ++i) {
        for (int ii = 0; ii < m->c; ++ii) {
	    m->values[i][ii] = values[i][ii];
	}
    }
}

void matrix_rotate(Matrix* m, Axis const a, Radian const r) {
    double a1 = a == x ? 1 : cos(r); double a2 = a == z ? -sin(r) : 0; double a3 = a == y ? sin(r) : 0;
    double b1 = a == z ? sin(r) : 0; double b2 = a == y ? 1 : cos(r); double b3 = a == x ? -sin(r) : 0;
    double c1 = a == y ? -sin(r) : 0; double c2 = a == x ? sin(r) : 0; double c3 = a == z ? 1 : cos(r);

    double rotation_array[3][3] = {{a1, a2, a3},
				   {b1, b2, b3},
				   {c1, c2, c3}};

    Matrix* rotation_matrix = matrix_init(3, 3);
    matrix_set(rotation_matrix, rotation_array);
    
    matrix_multiply(m, rotation_matrix, m);
    matrix_free(rotation_matrix);
}

void matrix_multiply(const Matrix* const m1, const Matrix* const m2, Matrix* const result) {
    assert(m1->c == m2->r);
    for (int r = 0; r < m1->r; ++r) {
	for (int c = 0; c < m2->c; ++c) {
	    double sum = 0;
	    for (int k = 0; k < m1->c; ++k) {
		sum += m1->values[r][k] * m2->values[k][c];
	    }
	    result->values[r][c] = sum;
	}
    }
}

double matrix_get_dot_product(const Matrix* const m1, const Matrix* const m2) {
    assert(m1->c == m2->c);
    double result = 0.0;
    for (int i = 0; i < m1->c; i++)
        result += m1->values[0][i] * m2->values[0][i];
    return result;
}

void matrix_project(const Matrix* const m,
		    const double dist_screen,
		    const double dist_shape) {
    m->values[0][0] = dist_screen * m->values[0][0] / (dist_shape + m->values[0][2]);
    m->values[0][1] = dist_screen * m->values[0][1] / (dist_shape + m->values[0][2]);
}

void matrix_set_normal(const Matrix* const m,
		       const double u,
		       const double v,
		       Matrix* const normal) {
    Matrix* m2 = matrix_init(1, 3);
    double f[1][3] = {{cos(u), sin(u), 0}};
    matrix_set(m2, f);
    matrix_multiply(m2, m, normal);
    matrix_free(m2);
}

double matrix_get_luminance(Matrix* const m,
			   Matrix* const normal,
			   const Matrix* const light_source,
			   const double u,
			   const double v) {
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
