//gcc -Wall -O2 stef.c -S -DN=100
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void getWallTime(double *watch) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *watch = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main () {
    
    float* oldImage = malloc(N * M * sizeof(float));
    float* newImage = malloc(N * M * sizeof(float));
    
    int i = 0;
    int j = 0;

    double timeStart,timeEnd;

    if (!oldImage) {
        printf("Memory not allocated for oldImage.\n"); 
        exit(1);
    }
    if (!newImage) {
        printf("Memory not allocated for newImage.\n"); 
        free(oldImage);
        exit(1);   
    }
    // initialize the arrays so they are preempted in the cache 
    // for (i = 0; i < N; i++) {
    //     for (j = 0; j < M; j++) {
    //         oldImage[i * M + j] = i;
    //         newImage[i * M + j] = i * 2.0;
    //     }
    // }
    for (i = 0; i < N * M; i++) {
        oldImage[i] = i;
        newImage[i] = i * 2.0;
    }
    getWallTime(&timeStart);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (i == 0) {
                // first row and first column
                if (j == 0) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j + 1] * 0.5);
                // first row and median column
                } else if (j <= M - 2) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j - 1] * 0.5)
                        + (oldImage[(i + 1) * M + j + 1] * 0.5);
                // first row and last column
                } else {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j - 1] * 0.5);
                }
            } else if (i <= N - 2) {
                // median row and first column
                if (j == 0) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j + 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j + 1] * 0.5);
                // median row and median column
                } else if (j <= M - 2) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j - 1] * 0.5)
                        + (oldImage[(i + 1) * M + j + 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j - 1] * 0.5)
                        + (oldImage[(i - 1) * M + j + 1] * 0.5);
                // median row and last column
                } else {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[(i + 1) * M + j] * 0.5)
                        + (oldImage[(i + 1) * M + j - 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j - 1] * 0.5);
                }
            } else {
                // last row and first column
                if (j == 0) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j + 1] * 0.5);
                // last row and median column
                } else if (j <= M - 2) {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[i * M + j + 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j - 1] * 0.5)
                        + (oldImage[(i - 1) * M + j + 1] * 0.5);
                // last row and last column
                } else {
                    newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                        + (oldImage[i * M + j - 1] * 0.5)
                        + (oldImage[(i - 1) * M + j] * 0.5)
                        + (oldImage[(i - 1) * M + j - 1] * 0.5);
                }
            }
        }
    }

    getWallTime(&timeEnd);
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%0.4f ", oldImage[i * M + j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------\n");
    printf("\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%0.4f ", newImage[i * M + j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Megaflops: ");
    printf("%f\n", (2.0 * (double)N * M) / ((timeEnd - timeStart) * 1e6));
    
    printf("Time took: ");
    printf("%f",timeEnd - timeStart);

    //free the memory
    free(oldImage);
    free(newImage);

    return 0;
}