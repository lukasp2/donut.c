#ifndef MATRIX_H
#define MATRIX_H

struct Matrix {
    const int r;
    const int c;
    double** values;
}; typedef struct Matrix Matrix;

enum Axis {
    x,
    y,
    z
}; typedef enum Axis Axis;

typedef double Radian;

// allocated memory for a matrix
Matrix* matrix_init(int const r, int const c);

// free allocated memory
void matrix_free(Matrix* m);

// fills matrix with values
void matrix_set(Matrix* m,
		double const values[m->r][m->c]);

// rotates matrix m around axis by r radians
void matrix_rotate(Matrix* m,
		   Axis const axis,
		   Radian const r);

// multiplies matrices m1 and m2, puts result in result
void matrix_multiply(const Matrix* const m1,
		     const Matrix* const m2,
		     Matrix* const result);

// calculates the dot product of v1 and v2 of size k
double matrix_get_dot_product(const Matrix* const m1,
			     const Matrix* const m2);

// projects 3D point on 2D plane
void matrix_project(const Matrix* const m,
		    const double dist_screen,
		    const double dist_shape);

// gets a normal vector for a point on the shape given u, v
void matrix_set_normal(const Matrix* const m,
		       const double u,
		       const double v,
		       Matrix* const normal);

// gets light intensity for a point
double matrix_get_luminance(Matrix* const m,
			   Matrix* const normal,
			   const Matrix* const light_source,
			   const double u,
			   const double v);

// prints a matrix
void matrix_print(const Matrix* const m, const char* const str);

#endif /* MATRIX_H */
