#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <climits> 
using namespace std;

void select_sort(int n, vector<int>& a) {
    // 选择排序
    for (int i = 0; i < n - 1; ++i) {
        int min_val = INT_MAX;
        int r = -1;
        for (int j = i + 1; j < n; ++j) {
            if (min_val > a[j]) {
                min_val = a[j];
                r = j;
            }
        }
        if (a[i] > min_val) {
            swap(a[i], a[r]);
        }
    }
}

void bubble_sort(int n, vector<int>& a) {
    // 冒泡排序
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void bubble_sort_better(int n, vector<int>& a) {
    // 优化的冒泡排序
    int left = 0;
    int right = n;
    while (left < right) {
        for (int i = left + 1; i < right; ++i) {
            if (a[i] < a[i - 1]) {
                swap(a[i], a[i - 1]);
            }
        }
        right--;
        for (int i = right - 1; i > left; --i) {
            if (a[i] < a[i - 1]) {
                swap(a[i], a[i - 1]);
            }
        }
        left++;
    }
}

void insert_sort(int n, vector<int>& a) {
    // 插入排序
    for (int j = 1; j < n; ++j) {
        int key = a[j];
        int i = j - 1;
        while (i >= 0 && a[i] > key) {
            a[i + 1] = a[i];
            i--;
        }
        a[i + 1] = key;
    }
}

void merge(int start, int mid, int end, vector<int>& a, vector<int>& tmp) {
    // 归并排序合并函数
    int pos = start;
    int l = start;
    int r = mid + 1;
    while (l <= mid && r <= end) {
        if (a[l] < a[r]) {
            tmp[pos] = a[l];
            l++;
        } else {
            tmp[pos] = a[r];
            r++;
        }
        pos++;
    }
    while (l <= mid) {
        tmp[pos] = a[l];
        l++;
        pos++;
    }
    while (r <= end) {
        tmp[pos] = a[r];
        r++;
        pos++;
    }
    for (int i = start; i <= end; ++i) {
        a[i] = tmp[i];
    }
}

void merge_sort_impl(int left, int right, vector<int>& a, vector<int>& tmp) {
    // 归并排序
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort_impl(left, mid, a, tmp);
        merge_sort_impl(mid + 1, right, a, tmp);
        merge(left, mid, right, a, tmp);
    }
}

void merge_sort(int n, vector<int>& a, vector<int>& tmp) {
    merge_sort_impl(0, n - 1, a, tmp);
}

void quicksort_impl(int l, int r, vector<int>& a) {
    // 快速排序
    if (l >= r) return;
    int base = a[l];
    int left = l;
    int right = r;
    while (left < right) {
        while (base <= a[right] && left < right) {
            right--;
        }
        a[left] = a[right];
        while (base >= a[left] && left < right) {
            left++;
        }
        a[right] = a[left];
    }
    a[left] = base;
    quicksort_impl(l, left - 1, a);
    quicksort_impl(left + 1, r, a);
}

void quicksort(int n, vector<int>& a) {
    quicksort_impl(0, n - 1, a);
}

void write_to_file(const string& file_name, const string& data) {
    // 将排序结果写入文件
    ofstream outfile(file_name, ios::app);
    outfile << data;
    outfile.close();
}

string time_sorting_algorithm(void (*sort_func)(int, vector<int>&), int n, vector<int>& a, const string& sort_name, int iteration, vector<int>& tmp) {
    // 执行排序并记录时间
    clock_t start_time = clock();
    
    sort_func(n, a);
    
    clock_t end_time = clock();
    double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;
    return "规模" + to_string(n) + " 第" + to_string(iteration + 1) + "次" + sort_name + "用时" + to_string(time_taken) + "秒\n";
}

int main() {
    vector<int> sizes = {10000, 20000, 30000, 40000, 50000};
    
    // 创建目录用于保存结果
    system("mkdir -p sort_results");
    
    // 文件路径
    string select_file = "sort_results/select_sort_times.txt";
    string bubble_file = "sort_results/bubble_sort_times.txt";
    string insert_file = "sort_results/insert_sort_times.txt";
    string merge_file = "sort_results/merge_sort_times.txt";
    string quick_file = "sort_results/quick_sort_times.txt";

    for (int n : sizes) {
        // 清空文件内容
        ofstream(select_file, ios::trunc).close();
        ofstream(bubble_file, ios::trunc).close();
        ofstream(insert_file, ios::trunc).close();
        ofstream(merge_file, ios::trunc).close();
        ofstream(quick_file, ios::trunc).close();

        for (int i = 0; i < 20; ++i) {
            srand(time(0)); // 使用系统时间生成随机种子
            vector<int> ori(n);
            for (int j = 0; j < n; ++j) {
                ori[j] = rand() % 1000000000;  // 生成随机数据
            }

            // 选择排序
            vector<int> a = ori;
            string result = time_sorting_algorithm(select_sort, n, a, "选择排序", i, a);
            write_to_file(select_file, result);

            // 冒泡排序
            a = ori;
            result = time_sorting_algorithm(bubble_sort, n, a, "冒泡排序", i, a);
            write_to_file(bubble_file, result);

            // 插入排序
            a = ori;
            result = time_sorting_algorithm(insert_sort, n, a, "插入排序", i, a);
            write_to_file(insert_file, result);

            // 归并排序
            a = ori;
            vector<int> tmp(n);
            clock_t start_time = clock();
            merge_sort(n, a, tmp);
            clock_t end_time = clock();
            double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;
            result = "规模" + to_string(n) + " 第" + to_string(i + 1) + "次归并排序用时" + to_string(time_taken) + "秒\n";
            write_to_file(merge_file, result);

            // 快速排序
            a = ori;
            result = time_sorting_algorithm(quicksort, n, a, "快速排序", i, a);
            write_to_file(quick_file, result);
        }
    }

    return 0;
}