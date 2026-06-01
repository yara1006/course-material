#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

// 常量定义
const int MAX_RANGE = 10000;
const double MAX_DISTANCE = 1000000.0;

// 定义一个新的点类
class Coordinates {
public:
    int x, y, index;
};

// 全局变量定义
int totalPoints = 0;
vector<Coordinates> points;
double closestDistanceSquared = MAX_DISTANCE;
double closestDistance = MAX_DISTANCE;
int resultPoint1 = -1, resultPoint2 = -1;

// 随机生成点集合
void GeneratePointsRandomly() {
    for (int i = 0; i < totalPoints; ++i) {
        points[i].x = rand() % MAX_RANGE;
        points[i].y = rand() % MAX_RANGE;
        points[i].index = i;
    }
}

// 初始化点集
void Initialize(int pointCount) {
    totalPoints = pointCount;
    points.resize(totalPoints);
    GeneratePointsRandomly();
    closestDistance = MAX_DISTANCE;
    closestDistanceSquared = MAX_DISTANCE;
}

// 计算两点之间的距离
double ComputeDistance(Coordinates a, Coordinates b) {
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

// 更新最近的点对
void UpdateResult(Coordinates a, Coordinates b) {
    double distanceSquared = ComputeDistance(a, b);
    if (0.01 < closestDistanceSquared - distanceSquared) {
        closestDistanceSquared = distanceSquared;
        resultPoint1 = a.index;
        resultPoint2 = b.index;
        closestDistance = sqrt(closestDistanceSquared);
    }
}

// 合并排序过程
void MergeSort(int left, int mid, int right) {
    vector<Coordinates> temp(right - left + 1);
    int leftIndex = left, rightIndex = mid + 1, tempIndex = 0;
    while (leftIndex <= mid && rightIndex <= right) {
        if (points[leftIndex].y < points[rightIndex].y)
            temp[tempIndex++] = points[leftIndex++];
        else
            temp[tempIndex++] = points[rightIndex++];
    }
    while (leftIndex <= mid) temp[tempIndex++] = points[leftIndex++];
    while (rightIndex <= right) temp[tempIndex++] = points[rightIndex++];
    for (int i = left; i <= right; ++i)
        points[i] = temp[i - left];
}

// 递归分治算法
void RecursionProcess(int left, int right) {
    if (left == right) return;
    int mid = (left + right) / 2;
    double middleX = points[mid].x;
    RecursionProcess(left, mid);
    RecursionProcess(mid + 1, right);
    MergeSort(left, mid, right);
    vector<Coordinates> temp(right - left + 1);
    int tempSize = 0;
    for (int i = left; i <= right; ++i) {
        if (abs(points[i].x - middleX) < closestDistance) {
            for (int j = tempSize - 1; j >= 0; --j) {
                if (points[i].y - temp[j].y >= closestDistance) break;
                UpdateResult(points[i], temp[j]);
            }
            temp[tempSize++] = points[i];
        }
    }
}

// 分治算法实现
void DivideAndConquer(int left, int right) {
    sort(points.begin() + left, points.begin() + right + 1, [](Coordinates &a, Coordinates &b) { return a.x < b.x; });
    RecursionProcess(left, right);
}


int main() {
    vector<int> pointCounts = {100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000};
    srand(time(nullptr));

    for (int N : pointCounts) {
        Initialize(N);
        cout << "Point Set Size: " << N << endl;
        auto start = high_resolution_clock::now();
        DivideAndConquer(0, totalPoints - 1);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Execution Time: " << duration.count() << " ms" << endl;
        closestDistance = MAX_DISTANCE;
    }

    return 0;
}
