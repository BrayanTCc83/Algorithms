#include <stdio.h>
#include "global.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>

    long long current_millis() {
        LARGE_INTEGER freq, counter;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&counter);
        return (counter.QuadPart * 1000) / freq.QuadPart;
    }

#else
    #include <sys/time.h>

    long long current_millis() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (long long)(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    }
#endif

int* read_fixed_integers(int n) {
    int* arr = malloc(n * sizeof(int));
    if (!arr) return NULL;

    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Failed to read integer #%d\n", i + 1);
            free(arr);
            return NULL;
        }
    }

    return arr;
}

void print_array(int n, int *array) {
    printf("Array: ");
    for(int i = 0; i < n; i++) {
        pritnf("%d ", array[i]);
    }
    printf("\n");
}