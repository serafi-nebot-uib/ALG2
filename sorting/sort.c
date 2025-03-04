#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "colors.h"

#define MAX 100
#define MIN 0
#define swap(a, b) { typeof(a) t = a; a = b; b = t; }

void arr_print(int *arr, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) printf("%d ", arr[i]);
    printf("\n");
}

void arr_cpy(int *src, int *dst, int len) {
    for (unsigned int i = 0; i < len; i++) dst[i] = src[i];
}

double time_elapsed_ms(clock_t start, clock_t end) {
    return (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
}

bool is_ordered(int *arr, unsigned int len) {
    for (unsigned int i = 0; i < len-1; i++)
        if (arr[i] > arr[i+1]) return false;
    return true;
}

void test(const char *name, void (*f)(int *, unsigned int), int *src, unsigned int len) {
    int *arr = (int *) malloc(len * sizeof(*src));
    if (arr == NULL) return;
    arr_cpy(src, arr, len);

    clock_t start = clock();
    f(arr, len);
    clock_t end = clock();

    const char *state = is_ordered(arr, len) ? GRN "PASSED" RESET : RED "FAILED" RESET;
    printf("%10s %10.3f ms %s\n", name, time_elapsed_ms(start, end), state);

    free(arr);
}

void selection(int *arr, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        unsigned int p = i;
        for (unsigned int j = i; j < len; j++) if (arr[j] < arr[p]) p = j;
        if (p != i) swap(arr[i], arr[p]);
    }
}

void _quicksort(int *arr, unsigned int l, unsigned int r) {
    if ((int)(r - l) < 1) return;
    unsigned int s = l, p = r;
    for (unsigned int i = s; i < p; i++) {
        if (arr[i] >= arr[p]) continue;
        swap(arr[s], arr[i]);
        s++;
    }
    swap(arr[s], arr[p]);
    _quicksort(arr, l, s-1);
    _quicksort(arr, s+1, r);
}

void quicksort(int *arr, unsigned int len) {
    _quicksort(arr, 0, len-1);
}

int compare(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

void qsort_internal(int *arr, unsigned int len) {
    qsort(arr, len, sizeof(*arr), compare);
}

int main(int argc, char **argv) {
    if (argc < 2) return -1;

    unsigned int n = atoi(argv[1]);
    int *src = (int *) malloc(n * sizeof(int));
    if (src == NULL) return -1;

    // srand(0);
    srand(time(NULL));
    for (unsigned int i = 0; i < n; i++) src[i] = rand() % (MAX - MIN + 1) + MIN;

    test("selection", selection, src, n);
    test("quicksort", quicksort, src, n);
    test("qsort", qsort_internal, src, n);

    free(src);

    return 0;
}
