#include "Matrix.h"

int main()
{
	Matrix* mat = Matrix_new();
	AddColumns(mat, 4);
	AddRows(mat, 2);
	PrintfMatrix(mat);
}