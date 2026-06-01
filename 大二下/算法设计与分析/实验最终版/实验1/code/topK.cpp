#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

const int MAX_ELEMENTS = 1e8 + 10;
int RANGES[101] = {100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
char SORT_METHODS[10][20] = {"NlogN", "NK", "NlogK", "NC"};


int total_elements, sort_method;
int data_array[MAX_ELEMENTS];
int temp_data[MAX_ELEMENTS];
int heap_array[MAX_ELEMENTS];

int partition_array(int arr[], int begin, int end) {
    int pivot = arr[begin];
    int left = begin, right = end;
    while (left < right) {
        while (arr[right] < pivot && left < right) right--;
        if (left < right) arr[left++] = arr[right];
        while (arr[left] > pivot && left < right) left++;
        if (left < right) arr[right--] = arr[left];
    }
    arr[left] = pivot;
    return left;
}

void quicksort(int arr[], int begin, int end) {
    // 快速排序
    if (end <= begin) return;
    int pivot_idx = partition_array(arr, begin, end);
    quicksort(arr, begin, pivot_idx - 1);
    quicksort(arr, pivot_idx + 1, end);
}

void selection_sort(int arr[], int begin, int end, int k) {
    // 选择排序
    for (int i = begin; i < begin + k; i++) {
        int max_idx = i;
        for (int j = i + 1; j <= end; j++)
            if (arr[max_idx] < arr[j]) max_idx = j;
        swap(arr[max_idx], arr[i]);
    }
}

void heapsort(int arr[], int begin, int end, int k) {
    // 堆排序（小顶堆）
    int count = 0;
    for (int i = begin; i <= end; i++) {
        if (count == k) {
            if (heap_array[1] >= arr[i]) continue;
            else {
                int parent = 1;
                heap_array[parent] = arr[i];
                while (parent < k) {
                    int child = parent << 1;
                    if (child > k) break;
                    if (child < k && heap_array[child] > heap_array[child + 1]) child++;
                    if (heap_array[child] >= heap_array[parent]) break;
                    swap(heap_array[parent], heap_array[child]);
                    parent = child;
                }
            }
        } else {
            int child = count + 1;
            heap_array[child] = arr[i];
            while (child != 1) {
                int parent = child >> 1;
                if (heap_array[parent] <= heap_array[child]) break;
                swap(heap_array[parent], heap_array[child]);
                child = parent;
            }
            count++;
        }
    }
}

void random_partition_sort(int arr[], int begin, int end, int k) {
    int pivot_idx = partition_array(arr, begin, end);
    if (pivot_idx == k + 1 || pivot_idx == k) return;
    else if (pivot_idx <= k) random_partition_sort(arr, pivot_idx + 1, end, k);
    else random_partition_sort(arr, begin, pivot_idx - 1, k);
}

void generate_random_data(int arr[], int n, int low, int high) {
    int range = high - low + 1;
    for (int i = 1; i <= n; i++)
        arr[i] = (long long)rand() * rand() * rand() % range + low;
    for (int i = 1; i <= n; i++)
        arr[i] = temp_data[i];
}

void initialize_temp_data() {
    int range = 2e9 + 10; // 20亿
    for (int i = 1; i <= 1e8; i++)
        temp_data[i] = (long long)rand() * rand() * rand() % range + 1;
}

int sort_and_measure_time(int n, int type, int k) {
    generate_random_data(data_array, n, 1, MAX_ELEMENTS * 20); // 最高有大约20亿
    clock_t start_time = clock();
    if (type == 0) quicksort(data_array, 1, n);
    else if (type == 1) selection_sort(data_array, 1, n, k);
    else if (type == 2) heapsort(data_array, 1, n, k);
    else if (type == 3) random_partition_sort(data_array, 1, n, k);
    clock_t end_time = clock();
    return end_time - start_time;
}


int main() {
    srand(time(0));
    int time_cost;
    int k=10;
    for (int i = 0; i < 4; i++) {
        printf("%s\n", SORT_METHODS[i]);

        printf("K:%d\n", k);
        int actual_cost = 0;
        for (int j = 0; j < 10; j++) {
            actual_cost = sort_and_measure_time(RANGES[j], i, k);
            printf("Range:%d Cost:%d\n", RANGES[j], actual_cost);
        }
        printf("\n");
    }

    return 0;
}