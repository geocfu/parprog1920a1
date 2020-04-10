// gcc -O2 -Wall -msse2 sse.c -o sse -DN=10000 -DM=10000
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}

int main() {
    float *oldImage;
    float *newImage;
    float *pixelConstant;

    float newPixelSummary;
    
    __m128 newImagePeripheralPixelSummary, newImageNewPixelValue;
    double timeStart, timeEnd;
    int i, j, k;

    i = posix_memalign((void **)&oldImage, 16, N * M * sizeof(float));
    if (i != 0) {
        exit(1);
    }

    i = posix_memalign((void **)&newImage, 16, N * M * sizeof(float));
    if (i != 0) {
        free(oldImage);
        exit(1);
    }

    i = posix_memalign((void **)&pixelConstant, 16, 4 * sizeof(float));
    if (i != 0) {
        free(oldImage);
        free(newImage);
        exit(1);
    }
    
    // init input (and output) matrices
    for (i = 0; i < N * M; i++) {
        oldImage[i] = i;
        newImage[i] = 1.0;
    }

    for (i = 0; i < 4; i++) {
        pixelConstant[i] = 0.5;
    }

    get_walltime(&timeStart);
    for (i = 1; i < N - 1; i ++) {
        for (j = 1; j < M - 1; j++) {
            newImagePeripheralPixelSummary = 
                _mm_mul_ps(
                    _mm_add_ps(
                        _mm_set_ps(oldImage[(i - 1) * M + j - 1],
                            oldImage[(i - 1) * M + j],
                            oldImage[(i - 1) * M + (j + 1)],
                            oldImage[i * M + j - 1]
                        ),
                        _mm_set_ps(oldImage[(i + 1) * M + j - 1],
                            oldImage[(i + 1) * M + j],
                            oldImage[(i + 1) * M + (j + 1)],
                            oldImage[i * M + (j + 1)]
                        )
                    ),
                    _mm_load_ps(pixelConstant)
                );
            for (k = 0; k < 4; k++) {
                newPixelSummary += newImagePeripheralPixelSummary[k];
            }
            
            newImageNewPixelValue = _mm_mul_ps(_mm_set_ps1(oldImage[i * M + j]), _mm_set_ps1(5.0));
            newPixelSummary += newImageNewPixelValue[0];
            newImage[i * M + j] = newPixelSummary;
            
            newPixelSummary = 0;
        }
    }
    get_walltime(&timeEnd);
    
    printf("Megaflops: %f\n", (2.0 * (double)N * M) / ((timeEnd - timeStart) * 1e6));
    printf("Computation time = %f sec\n", (timeEnd - timeStart));

    // free matrices
    free(oldImage);
    free(newImage);
    free(pixelConstant);

    return 0;
}