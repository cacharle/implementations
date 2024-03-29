#include <stdlib.h>
#include <string.h>
#include "sorting.h"
#include "helper.h"
#include "vector.h"
#include "heap.h"

static void merge_array(void *base, void *left, void *right, size_t base_len,
                        size_t size, int (*compar)(const void *, const void *));
static void quicksort_rec(void *base, int lo, int hi, size_t size,
		                  int (*compar)(const void *, const void *));
static int quicksort_partition(void *base, int lo, int hi, size_t size,
		                       int (*compar)(const void *, const void *));

void bubble_sort(void *base, size_t nmemb, size_t size,
		         int (*compar)(const void *, const void *))
{
    for (int i = 0; i < nmemb; i++)
        for (int j = 0; j < nmemb - i - 1; j++)
            if (compar(base + j * size, base + j * size + size) > 0)
                swap(base + j * size, base + j * size + size, size);
}

void insertion_sort(void *base, size_t nmemb, size_t size,
		            int (*compar)(const void *, const void *))
{
    for (int i = 0; i < nmemb; i++)
        for (int j = i; j > 0 && compar(base + (j - 1) * size, base + j * size) > 0; j--)
            swap(base + (j - 1) * size, base + j * size, size);
}

void shell_sort(void *base, size_t nmemb, size_t size,
		        int (*compar)(const void *, const void *))
{
    size_t gap;
    int i, j;

    for (gap = nmemb / 2; gap >= 1; gap /= 2)
        for (i = 0; i + gap <= nmemb; i++)
        {
            j = i;
            while (j >= gap && compar(base + (j - gap) * size, base + j * size) > 0)
            {
                swap(base + (j - gap) * size, base + j * size, size);
                j -= gap;
            }
        }
}

void selection_sort(void *base, size_t nmemb, size_t size,
		            int (*compar)(const void *, const void *))
{
    int index_min;

    for (int i = 0; i < nmemb - 1; i++)
    {
        index_min = i;
        for (int j = i + 1; j < nmemb; j++)
        {
            if (compar(base + j * size, base + index_min * size) < 0)
                index_min = j;
        }
        if (index_min != i)
            swap(base + i * size, base + index_min * size, size);
    }
}

void merge_sort(void *base, size_t nmemb, size_t size,
		        int (*compar)(const void *, const void *))
{
    int mid;
    void *left, *right;

    if (nmemb < 2)
        return ;
    mid = nmemb / 2;
    if ((left = malloc(mid * size)) == NULL)
        return ;
    if ((right = malloc((nmemb - mid) * size)) == NULL)
        return ;
    memcpy(left, base, mid * size);
    memcpy(right, base + mid * size, (nmemb - mid) * size);

    merge_sort(left, mid, size, compar);
    merge_sort(right, nmemb - mid, size, compar);
    merge_array(base, left, right, nmemb, size, compar);
    free(left);
    free(right);
}

static void merge_array(void *base, void *left, void *right, size_t base_len,
                        size_t size, int (*compar)(const void *, const void *))
{
    int base_index = 0;
    int left_i = 0;
    int right_i = 0;
    int left_len = base_len / 2;
    int right_len = base_len - left_len;

    while (left_i < left_len && right_i < right_len)
    {
        if (compar(left + left_i * size, right + right_i * size) <= 0)
            memcpy(base + base_index * size,
                   left + left_i++ * size, size);
        else
            memcpy(base + base_index * size,
                   right + right_i++ * size, size);
        base_index++;
    }
    while (left_i < left_len)
        memcpy(base + base_index++ * size,
               left + left_i++ * size, size);
    while (right_i < right_len)
        memcpy(base + base_index++ * size,
               right + right_i++ * size, size);
}

void quicksort(void *base, size_t nmemb, size_t size,
		       int (*compar)(const void *, const void *))
{
    quicksort_rec(base, 0, nmemb - 1, size, compar);
}

static void quicksort_rec(void *base, int lo, int hi, size_t size,
		                  int (*compar)(const void *, const void *))
{
    size_t pivot_i;

    if (lo >= hi)
        return ;
    pivot_i = quicksort_partition(base, lo, hi, size, compar);
    quicksort_rec(base, lo, pivot_i - 1, size, compar);
    quicksort_rec(base, pivot_i + 1, hi, size, compar);
}

static int quicksort_partition(void *base, int lo, int hi, size_t size,
		                       int (*compar)(const void *, const void *))
{
    void *pivot = base + hi * size;
    int pivot_i = lo;

    for (int i = lo; i < hi; i++)
    {
        if (compar(base + i * size, pivot) < 0)
        {
            swap(base + pivot_i * size, base + i * size, size);
            pivot_i++;
        }
    }
    swap(base + pivot_i * size, pivot, size);
    return pivot_i;
}

void heapsort(void *base, size_t nmemb, size_t size,
		      int (*compar)(const void *, const void *))
{
    Heap *heap = heap_heapify(base, nmemb, size, compar);
    while (heap->size > 0)
    {
        /* print_int_array(heap->under, heap->size); */
        /* printf("-\n"); */
        /* print_int_array(&((int*)heap->under)[heap->size], nmemb - heap->size); */
        /* printf("----------\n"); */

        heap_stash_max(heap, compar);
    }
    memcpy(base, heap->under, nmemb * size);
}

void cocktail_sort(void *base, size_t nmemb, size_t size,
		           int (*compar)(const void *, const void *))
{
    int j;

    for (int lo = 0, hi = nmemb - 1; lo < hi; lo++, hi--)
    {
        /* printf("%d..%d\n", lo, hi); */
        for (j = lo; j < hi; j++)
            if (compar(base + j * size, base + (j + 1) * size + size) > 0)
                swap(base + j * size, base + (j + 1) * size + size, size);
        for (--j; j > lo; j--)
            if (compar(base + j * size, base + (j - 1) * size + size) < 0)
                swap(base + j * size, base + (j - 1) * size + size, size);
    }
}
