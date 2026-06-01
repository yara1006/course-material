#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <unordered_set>
#include <algorithm>
#include <random>
using namespace std;
using namespace std::chrono;

// 点结构体
struct Point {
    int x = 0;
    int y = 0;
};

// 按照x轴排序
bool cmp(Point a, Point b) {
    return a.x == b.x ? a.y > b.y : a.x > b.x;
}

// 全局变量
int n = 0;
vector<Point> points;

// 计算两点间的平方距离
double dist(const Point &p1, const Point &p2) {
    double dx = static_cast<double>(p1.x) - static_cast<double>(p2.x);
    double dy = static_cast<double>(p1.y) - static_cast<double>(p2.y);
    return dx * dx + dy * dy;
}

// 初始化点集
void init(int N) {
    n = N;
    points.resize(n);
        random_device rd;//用于生成随机数
		mt19937 gen(rd());//均匀分布，数据随机性较高
		uniform_int_distribution<>distrib(0, 1e9);
		for (int i = 0; i < n; i++) {//生成n个随机数据，范围（0，1e9）
			points[i].x = distrib(gen);
			points[i].y = distrib(gen);
		}
    sort(points.begin(), points.end(), cmp);  // 按照x坐标排序
}

// 暴力法计算最小距离
double calculate_point() {
    double min = 1e12;  // 使用较大的初始值，而不是 DBL_MAX
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double distance = dist(points[i], points[j]);
            if (distance < min) {
                min = distance;
            }
        }
    }
    return sqrt(min);  // 返回实际的最小距离
}

int main() {
    srand(time(0));  // 初始化随机种子

    // 定义点集规模列表
    vector<int> N_values = { 100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000};

    for (int N : N_values) {
        cout << "正在处理点集规模: " << N << endl;

        init(N);  // 初始化点集

        auto start = high_resolution_clock::now();  // 记录开始时间

        double result = calculate_point();  // 计算最小距离

        auto end = high_resolution_clock::now();  // 记录结束时间

        // 输出最小距离
        cout << "最小距离: " << result << endl;

        // 计算执行时间
        auto duration = duration_cast<microseconds>(end - start);
        cout << "计算时间: " << duration.count() << " 微秒" << endl;
        cout << "-----------------------------" << endl;
    }

    return 0;
}
