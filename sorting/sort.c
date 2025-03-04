#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define swap(a, b) { typeof(a) t = a; a = b; b = t; }

void selection(int *arr, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        unsigned int p = i;
        for (unsigned int j = i; j < len; j++)
            if (arr[j] < arr[p]) p = j;
        if (p != i) {
            unsigned int t = arr[i];
            arr[i] = arr[p];
            arr[p] = t;
        }
    }
}

void quicksort_partition(int *arr, unsigned int l, unsigned int r) {
    if ((int)(r - l) < 2) return;
    unsigned int s = l,  p = r, t = 0;
    for (unsigned int i = s; i < p; i++) {
        if (arr[i] >= arr[p]) continue;
        swap(arr[s], arr[i]);
        s++;
    }
    swap(arr[s], arr[p]);
    quicksort_partition(arr, l, s-1);
    quicksort_partition(arr, s+1, r);
}

void quicksort(int *arr, unsigned int len) {
    quicksort_partition(arr, 0, len-1);
}

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

void test(const char *name, void (f)(int *arr, unsigned int len), int *src, unsigned int len) {
    int *arr = (int *) malloc(len);
    if (arr == NULL) return;
    arr_cpy(src, arr, len);

    clock_t start = clock();
    f(arr, len);
    clock_t end = clock();
    printf("%10s %8.3f ms: ", name, time_elapsed_ms(start, end));
    arr_print(arr, len);

    free(arr);
}

int main(int argc, char **argv) {
    int src[] = { 5, 7, 1, 3, 8, 9, 2, 4, 6 };
    unsigned int n = sizeof(src) / sizeof(*src);
    printf("src: ");
    arr_print(src, sizeof(src) / sizeof(*src));

    test("selection", selection, src, n);
    test("quicksort", quicksort, src, n);

    return 0;
}
