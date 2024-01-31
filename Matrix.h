#pragma once
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
	float** data;
	unsigned int rowsAmount;
	unsigned int columnsAmount;

	pthread_mutex_t* mutex;
} Matrix;

extern Matrix* Matrix_new();
//TO DO:
//extern Matrix* Array2DToMatrix(float** array, int rows, int columns);
extern void Matrix_destroy(Matrix* matrix);

extern void AddRows(Matrix* matrix, int amount);
extern void AddColumns(Matrix* matrix, int amount);
extern void SetElement(Matrix* matrix, int rowInd, int columnInd, float value);

extern float GetElement(Matrix* matrix, int rowInd, int columnInd);
extern void GetMatrixSize(Matrix* matrix, int* row, int* column);

extern void PrintfMatrix(Matrix* matrix);

extern Matrix* MatrixMultiplication(const Matrix* matrixA, const Matrix* matrixB);