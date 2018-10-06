/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 *
 * Student Name:李珊 
 * Student ID:516030910175
 *
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int x1,x2,x3,x4,x5,x6,x7,x8;
    if(M==32){
        for(int k = 0; k<32 ;k+=8){
            for(int i = 0; i < 32 ;i++){
                x1 = A[i][k];
                x2 = A[i][k+1];
                x3 = A[i][k+2];
                x4 = A[i][k+3];
                x5 = A[i][k+4];
                x6 = A[i][k+5];
                x7 = A[i][k+6];
                x8 = A[i][k+7];
                B[k][i] = x1;
                B[k+1][i] = x2;
                B[k+2][i] = x3;
                B[k+3][i] = x4;
                B[k+4][i] = x5;
                B[k+5][i] = x6;
                B[k+6][i] = x7;
                B[k+7][i] = x8;
            }
        }

    }

    else if(M==64){
        for(int k = 0; k<32 ;k+=4){
            for(int i = 0; i < 32 ;i++){//divided
                x1 = A[i][k];
                x2 = A[i][k+1];
                x3 = A[i][k+2];
                x4 = A[i][k+3];

                B[k][i] = x1;
                B[k+1][i] = x2;
                B[k+2][i] = x3;
                B[k+3][i] = x4;

            }
        }

        for(int k = 0; k<32 ;k+=4){
            for(int i = 32; i < 64 ;i++){
                x1 = A[i][k];
                x2 = A[i][k+1];
                x3 = A[i][k+2];
                x4 = A[i][k+3];

                B[k][i] = x1;
                B[k+1][i] = x2;
                B[k+2][i] = x3;
                B[k+3][i] = x4;

            }
        }

        for(int k = 32; k<64 ;k+=4){
            for(int i = 32; i < 64 ;i++){
                x1 = A[i][k];
                x2 = A[i][k+1];
                x3 = A[i][k+2];
                x4 = A[i][k+3];
    
                B[k][i] = x1;
                B[k+1][i] = x2;
                B[k+2][i] = x3;
                B[k+3][i] = x4;
        
            }
        }

        for(int k = 32; k<64 ;k+=4){
            for(int i = 0; i < 32 ;i++){
                x1 = A[i][k];
                x2 = A[i][k+1];
                x3 = A[i][k+2];
                x4 = A[i][k+3];
        
                B[k][i] = x1;
                B[k+1][i] = x2;
                B[k+2][i] = x3;
                B[k+3][i] = x4;
  
            }
        }
    
    
    }
    
    else if(M==61){
        for(int i = 0; i<67 ;i+=8){ 
            for(int j = 0; j < 61 ;j+=5){
                for(int k = i; k < i+8 && k < 67; k++){
                    x1 = A[k][j];
                    x2 = A[k][j+1];
                    x3 = A[k][j+2];
                    x4 = A[k][j+3];
                    x5 = A[k][j+4];
   
                    B[j][k] = x1;
                    B[j+1][k] = x2;
                    B[j+2][k] = x3;
                    B[j+3][k] = x4;
                    B[j+4][k] = x5;

                }
            }
        }

            for(int i = 0; i < 67 ;i++){
                int k = 60;
                x1 = A[i][k];


                B[k][i] = x1;

              
            }

     

    }
    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

