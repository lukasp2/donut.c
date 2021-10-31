#ifndef MATRIX_H
#define MATRIX_H

struct Matrix {
    const int r;
    const int c;
    float** values;
}; typedef struct Matrix Matrix;

enum Axis {
    x,
    y,
    z
}; typedef enum Axis Axis;

typedef float Radian;

// allocated memory for a matrix
Matrix* matrix_init(int const r, int const c);

// rotates matrix m around axis by r radians
void matrix_rotate(Matrix* m,
		   Axis const axis,
		   Radian const r);

// fills matrix with values
void matrix_set(Matrix* m,
		float const values[m->r][m->c]);

// free allocated memory
void matrix_free(Matrix* m);

// multiplies matrices m1 and m2, puts result in result
void matrix_multiply(const Matrix* const m1,
		     const Matrix* const m2,
		     Matrix* const result);

// calculates the dot product of v1 and v2 of size k
float matrix_get_dot_product(const Matrix* const m1,
			     const Matrix* const m2);

// projects 3D point on 2D plane
void matrix_project(const Matrix* const m,
		    const float dist_screen,
		    const float dist_shape);

// gets a normal vector for a point on the shape given u, v
void matrix_set_normal(const Matrix* const m,
		       const float u,
		       const float v,
		       Matrix* const normal);

// gets light intensity for a point
float matrix_get_luminance(Matrix* const m,
			   Matrix* const normal,
			   const Matrix* const light_source,
			   const float u,
			   const float v);

// prints a matrix
void matrix_print(const Matrix* const m);

#endif /* MATRIX_H */
