#include "Matrix.h"
#include <string.h> 

struct RowColumnMultiplicationArgs
{
	Matrix* A;
	Matrix* B;
	int ColumnIndex;
	int RowIndex;
	float* Result;
};


Matrix* Matrix_new()
{
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
	matrix->data = (float**)calloc(0, sizeof(float*));
	matrix->columnsAmount = 0;
	matrix->rowsAmount = 0;

	pthread_mutex_init(&matrix->mutex, NULL);

	return matrix;
}

void Matrix_destroy(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->rowsAmount; i++)
	{
		free(matrix->data[i]);
	}
	free(matrix->data);

	pthread_mutex_destroy(&matrix->mutex);

	free(matrix);
	return;
}

void AddColumns(Matrix* matrix, int amount)
{
	if (amount < 1)
	{
		printf("AddColumns error: Amount of columns to add must be greater then 0\n");
		return;
	}

	if (pthread_mutex_lock(&matrix->mutex) != 0)
	{
		printf("AddColumns error: Can't aquire mutex. Columns weren't added\n");
		return;
	}

	for (size_t i = 0; i < matrix->rowsAmount; i++)
	{
		float* row_extend = realloc(matrix->data[i], (matrix->columnsAmount + amount) * sizeof(float));
		if (row_extend == NULL)
		{
			printf("AddColumns error: Can't realloc memory. Columns weren't added\n");
			return;
		}
		
		for (size_t j = matrix->columnsAmount; j < matrix->columnsAmount + amount; j++)
		{
			row_extend[j] = 0;
		}

		matrix->data[i] = row_extend;
	}
	matrix->columnsAmount += amount;

	pthread_mutex_unlock(&matrix->mutex);
}

void SetElement(Matrix* matrix, int rowInd, int columnInd, float value)
{
	if (rowInd >= matrix->rowsAmount)
	{
		printf("SetElement error: Row index is out of range\n");
		return;
	}
	if (columnInd >= matrix->columnsAmount)
	{
		printf("SetElement error: Row index is out of range\n");
		return;
	}
	matrix->data[rowInd][columnInd] = value;
}

float GetElement(Matrix* matrix, int rowInd, int columnInd)
{
	if (rowInd >= matrix->rowsAmount)
	{
		printf("SetElement error: Row index is out of range\n");
		return;
	}
	if (columnInd >= matrix->columnsAmount)
	{
		printf("SetElement error: Row index is out of range\n");
		return;
	}
	return matrix->data[rowInd][columnInd];
}

void GetMatrixSize(Matrix* matrix, int* rows, int* columns)
{
	*rows = matrix->rowsAmount;
	*columns = matrix->columnsAmount;
}

void PrintfMatrix(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->rowsAmount; i++)
	{
		for (size_t j = 0; j < matrix->columnsAmount; j++)
		{
			printf("%9.4f ", matrix->data[i][j]);
		}
		printf("\n");
	}
}

//Use only in MatrixMultiplication function
void* RowColumnMultiplication(void* ArgStruct)
{
	struct RowColumnMultiplicationArgs* pars = (struct RowColumnMultiplicationArgs*)ArgStruct;
	for (size_t i = 0; i < pars->B->rowsAmount; i++)
	{
		*pars->Result += GetElement(pars->A, pars->RowIndex, i) * GetElement(pars->B, i, pars->ColumnIndex);
	}
	pthread_exit(0);
}

Matrix* MatrixMultiplication(Matrix* matrixA, Matrix* matrixB)
{
	Matrix* matrixC = Matrix_new();
	AddColumns(matrixC, matrixB->columnsAmount);
	AddRows(matrixC, matrixA->rowsAmount);

	if (matrixB->rowsAmount != matrixA->columnsAmount)
	{
		printf("MatrixMultiplication error:	Matrices are incompatible for multiplication");
		return NULL;
	}

	pthread_t* thread = calloc(matrixA->rowsAmount* matrixB->columnsAmount, sizeof(pthread_t));

	for (size_t i = 0; i < matrixA->rowsAmount; i++)
	{
		for (int j = 0; j < matrixB->columnsAmount; j++)
		{
			struct RowColumnMultiplicationArgs* par = (struct RowColumnMultiplicationArgs*)malloc(sizeof(struct RowColumnMultiplicationArgs));
			par->A = matrixA;
			par->B = matrixB;
			par->ColumnIndex = j;
			par->RowIndex = i;
			par->Result = &matrixC->data[i][j];

			pthread_create(&thread[i* matrixA->rowsAmount + j], NULL, &RowColumnMultiplication, (void*)par);
		}
	}

	for (size_t i = 0; i < matrixA->rowsAmount* matrixB->columnsAmount; i++)
	{
		pthread_join(thread[i], NULL);
	}
	
	return matrixC;
}

void AddRows(Matrix* matrix, int amount)
{
	if (amount < 1)
	{
		printf("AddRows error: Amount of rows to add must be greater then 0\n");
		return;
	}

	if (pthread_mutex_lock(&matrix->mutex) != 0)
	{
		printf("AddRows error: Can't aquire mutex. Rows weren't added\n");
		return;
	}

	// Allocating more memory to store rows
	float** data_extend = realloc(matrix->data, (matrix->rowsAmount+ amount) * sizeof(float*));
	if (data_extend == NULL)
	{
		printf("AddRows error: Can't realloc memory. Rows weren't added\n");
		return;
	}

	// Allocating and initializing memory to store matrix values
	matrix->data = data_extend;
	for (size_t i = 0; i < amount; i++)
	{
		matrix->data[matrix->rowsAmount+i] = (float*)calloc(matrix->columnsAmount, sizeof(float));
	}
	matrix->rowsAmount += amount;

	pthread_mutex_unlock(&matrix->mutex);
}



