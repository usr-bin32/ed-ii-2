#include <cstdlib>
#include "../hashing/hashing.h"

// particiona o vetor e o rearranja de forma que todos os elementos anteriores ao pivô sejam menores que ele e todos os posteriores sejam maiores.
template <typename T>
int slice(T &arr, int first, int end) {
    int n = (end + first);
    int pivot = arr[n / 2].value;

    int i = first - 1, j = end + 1;

    do {
        do {
            i++;
        } while (arr[i].value < pivot);

        do {
            j--;
        } while (arr[j].value > pivot);

        if (i < j) {
            hash_node<int> aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
        }
    } while (i < j);
    return j;
}

template <typename T>
void quick_sort(T &arr, int first, int end) {
    if (first < end) {
        int s = slice(arr, first, end);
        // chamada recursiva para ordenar o subvetor de elementos menores
        quick_sort(arr, first, s);
        // chamada recursiva para ordenar o subvetor de elementos maiores
        quick_sort(arr, s + 1, end);
    }
}

template<typename T>
void quick_sort(T &arr, size_t size) {
   quick_sort(arr, 0, size - 1);
}