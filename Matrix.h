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
extern void Matrix_destroy(Matrix* matrix);

extern void AddRows(Matrix* matrix, int amount);
extern void AddColumns(Matrix* matrix, int amount);

extern void PrintfMatrix(Matrix* matrix);
