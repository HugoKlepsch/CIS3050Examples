%module gsl

%{
#include <gsl/gsl_matrix.h>
#include "gslMatrix.h"
%}

gsl_matrix *matrixAlloc(int nRows, int nCols);
void matrixFree(gsl_matrix *mat);
float matrixGetValue(gsl_matrix *mat, int row, int col);
void matrixSetValue(gsl_matrix *mat, int row, int col, float value);

