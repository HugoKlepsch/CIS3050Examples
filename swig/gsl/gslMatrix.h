/*
 * Header file to declare the GSL wrapper function names
 *
 * $Id: gslMatrix.h 365 2007-03-09 18:50:45Z andrew $
 */
#ifndef	__GSL_MATRIX_WRAPPER__
#define	__GSL_MATRIX_WRAPPER__

/*
 * Include all of our necessary header files
 */
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>


/*
 * Provide prototypes for the functions we are declaring
 */
gsl_matrix * matrixAlloc(int nRows, int nCols);
void matrixFree(gsl_matrix *matrix);
float matrixGetValue(gsl_matrix *matrix, int row, int col);
void matrixSetValue(gsl_matrix *matrix, int row, int col, float value);

#endif
