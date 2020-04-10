//gcc -O2 -Wall no-sse.c -o no-sse -DN=10000 -DM=10000
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
    for (i = 0; i < N * M; i++) {
        oldImage[i] = i;
        newImage[i] = 1.0;
    }

    getWallTime(&timeStart);
    for (i = 1; i < N - 1; i++) {
        for (j = 1; j < M - 1; j++) {
            newImage[i * M + j] = (oldImage[i * M + j] * 5.0)
                + (oldImage[i * M + j - 1] * 0.5)
                + (oldImage[i * M + j + 1] * 0.5)
                + (oldImage[(i + 1) * M + j] * 0.5)
                + (oldImage[(i + 1) * M + j - 1] * 0.5)
                + (oldImage[(i + 1) * M + j + 1] * 0.5)
                + (oldImage[(i - 1) * M + j] * 0.5)
                + (oldImage[(i - 1) * M + j - 1] * 0.5)
                + (oldImage[(i - 1) * M + j + 1] * 0.5);
        }
    }
    getWallTime(&timeEnd);

    printf("Megaflops: %f\n", (2.0 * (double)N * M) / ((timeEnd - timeStart) * 1e6));
    printf("Time took: %f\n", timeEnd - timeStart);

    //free the memory
    free(oldImage);
    free(newImage);

    return 0;
}