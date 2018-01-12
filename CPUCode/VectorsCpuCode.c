/**
 * Document: MaxCompiler Tutorial (maxcompiler-tutorial.pdf)
 * Chapter: 6      Example: 3      Name: Vectors
 * MaxFile name: Vectors
 * Summary:
 *    Streams a vector of integers to the dataflow engine and confirms that the
 *    returned stream contains the integers values doubled.
 * 
 * Matrix Multiplication
 * Matthew Pletcher, Fall 2017
 * Written using Vector tutorial example as a starting point
 */

#include <stdio.h>
#include <stdlib.h>
#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

int dotProduct(int m, int vectorA[m], int vectorB[m])
{
    int sum = 0;
    
    for(int i = 0; i < m; i++)
    {
        sum += vectorA[i] * vectorB[i];
    }
    return sum;
}

void multiply(int n, int m, int p, int matrixA[][m], int matrixB[][p], int matrixC[][p])
{
    //hand the accelerator the rows and columns to multiply
    for(int i = 0; i < n; i++)
    {
        int col[m];
        
        for(int l = 0; l < m; l++)
        {
            col[l] = matrixA[i][l];
        }
        
        for(int j = 0; j < p; j++)
        {
            int row[m];
            
            for(int k = 0; k < m; k++)
            {
                row[k] = matrixB[k][j];
            }
            
            int cVal[m];
            //Dot product with accelerator
            Vectors(m, col, row, cVal);
            int sum = 0;
            //can't figure out how to coalesce the values in the accelerator,
            //just going to add them all up with the CPU
            for(int i = 0; i < m; i++) {
                sum += cVal[i];
            }
            //dot product with CPU
            // cVal = dotProduct(m, col, row);
            matrixC[i][j] = sum;
        }
    }
}



// int main()
// {
// 	const int vectorSize = Vectors_vectorSize;
// 	const int streamSize = 16;
// 	size_t sizeBytes = vectorSize * streamSize * sizeof(uint32_t);
// 	uint32_t *inVector = malloc(sizeBytes);
// 	uint32_t *outVector = malloc(sizeBytes);
// 	uint32_t *expectedVector = malloc(sizeBytes);

// 	for (int i = 0; i < vectorSize * streamSize; i++) {
// 		inVector[i] = i;
// 	}

// 	for (int i = 0; i < vectorSize * streamSize; i++) {
// 		outVector[i] = 0;
// 	}

// 	VectorsCPU(streamSize, inVector, outVector);

// 	printf("\n");
// 	printf("Running DFE.\n");
// 	Vectors(streamSize, inVector, expectedVector);

// 	int status = check(streamSize, outVector, expectedVector);
// 	if (status)
// 		printf("Test failed.\n");
// 	else
// 		printf("Test passed OK!\n");

//     printf("\n");
// 	return status;
// }


int main()
{
    // const int vectorSize = Vectors_vectorSize;
    printf("\n");
	printf("Running DFE\n");
	
	int n, m, p;
	n = 2;
	m = 4;
	p = 3;
	
	
	int mat1[2][4] = {
	    {1,3,5,7},
	    {2,4,6,8},
	};
	
	int mat2[4][3] = {
	    {1,8,9},
	    {2,7,10},
	    {3,6,11},
	    {4,5,12}
	};
	
	int mat3[2][3] = {
	  {50,94,178},
	  {60,120,220}  
	};
	
	int result[2][3];
	
	multiply(n, m, p, mat1, mat2, result);
	
	printf("Does our function work?\n");
	bool res = true;
	
	for(int i = 0; i < n; i++)
	{
	    for(int j = 0; j < p; j++)
	    {
	        res = res && (result[i][j] == mat3[i][j]);
	    }
	}
	printf(res ? "true" : "false");
	printf("\n");
	
	printf("result array:\n");
	for(int i = 0; i < n; i++)
	{
        for(int j = 0; j < p; j++)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    printf("expected array:\n");
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < p; j++)
        {
            printf("%d ", mat3[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
	return 0;
}
