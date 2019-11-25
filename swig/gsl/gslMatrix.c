/*
 * A C file containing functions that will wrap up the GSL library
 * interface in a much simpler way.
 *
 * $Id: gslMatrix.c 365 2007-03-09 18:50:45Z andrew $
 */
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>

#include "gslMatrix.h"

/*
 * allocate a matrix and ensure it is filled with zeros
 */
gsl_matrix *
matrixAlloc(int nRows, int nCols)
{
    gsl_matrix *matrix;

    matrix = gsl_matrix_alloc(nRows, nCols);
    gsl_matrix_set_zero(matrix);

    return matrix;
}

/*
 * delete a matrix
 */
void
matrixFree(gsl_matrix *matrix)
{
    if (matrix != NULL)
	gsl_matrix_free(matrix);
}

/*
 * accessor to get a value (really just to make the
 * name work without underscores)
 */
float
matrixGetValue(gsl_matrix *matrix, int row, int col)
{
    if (matrix != NULL)
	return gsl_matrix_get(matrix, row, col);

    return (-log(0));
}

/*
 * accessor to get a value (really just to make the
 * name work without underscores)
 */
void
matrixSetValue(gsl_matrix *matrix, int row, int col, float value)
{
    if (matrix != NULL)
	gsl_matrix_set(matrix, row, col, value);
}

