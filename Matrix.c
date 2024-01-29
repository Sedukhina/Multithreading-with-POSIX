#include "Matrix.h"

Matrix* Matrix_new()
{
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
	matrix->data = (float**)malloc(0, sizeof(float*));
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
}

void AddColumns(Matrix* matrix, int amount)
{
	if (amount < 1)
	{
		printf("AddColumns error: Amount must be greater then 0\n");
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

void PrintfMatrix(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->rowsAmount; i++)
	{
		for (size_t j = 0; j < matrix->columnsAmount; j++)
		{
			printf("%3f ", matrix->data[i][j]);
		}
		printf("\n");
	}
}

void AddRows(Matrix* matrix, int amount)
{
	if (amount < 1)
	{
		printf("AddRows error: Amount must be greater then 0\n");
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



