#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void sum_regular(int arr[], int size) {
    long long sum = 0;
    clock_t start = clock();

    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Regular loop: Sum = %lld\n", sum);
    printf("Time taken by regular loop: %f seconds\n", time_taken);
}

void sum_unrolled(int arr[], int size) {
    long long sum = 0;
    clock_t start = clock();

    int i;
    for (i = 0; i < size - 3; i += 4) {
        sum += arr[i] + arr[i+1] + arr[i+2] + arr[i+3];
    }

    // Handle remaining elements (if size not divisible by 4)
    for (; i < size; i++) {
        sum += arr[i];
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Unrolled loop: Sum = %lld\n", sum);
    printf("Time taken by loop unrolling: %f seconds\n", time_taken);
}

int main() {
    int *arr = (int*)malloc(SIZE * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Fill array with 1s
    for (int i = 0; i < SIZE; i++) {
        arr[i] = 1;
    }

    printf("----- Regular Loop -----\n");
    sum_regular(arr, SIZE);

    printf("\n----- Loop Unrolling -----\n");
    sum_unrolled(arr, SIZE);

    free(arr);
    return 0;
}
