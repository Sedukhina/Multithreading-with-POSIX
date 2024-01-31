#include "Matrix.h"

int main()
{
	Matrix* matA = Matrix_new();
	AddColumns(matA, 2);
	AddRows(matA, 2);

	SetElement(matA, 0, 0, 1);
	SetElement(matA, 0, 1, 2);
	SetElement(matA, 1, 0, 3);
	SetElement(matA, 1, 1, 4);

	Matrix* matB = Matrix_new();
	AddColumns(matB, 2);
	AddRows(matB, 2);

	SetElement(matB, 0, 0, 1);
	SetElement(matB, 0, 1, 2);
	SetElement(matB, 1, 0, 3);
	SetElement(matB, 1, 1, 4);

	Matrix* matC = MatrixMultiplication(matA, matB);

	PrintfMatrix(matC);

	Matrix_destroy(matA);
	Matrix_destroy(matB);
	Matrix_destroy(matC);

	return 0;
}