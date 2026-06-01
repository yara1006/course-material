#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_NODE = 1e6 + 10;
const int MAX_EDGE = 3e7 + 10;

int vertexCount, edgeCount;

struct GraphEdge {
    int from, to;
    bool isBridge;

    GraphEdge(int f = 0, int t = 0) : from(f), to(t), isBridge(false) {}
};

GraphEdge edges[MAX_EDGE >> 1];
int bridgeCount = 0;

// 图的邻接表表示
int head[MAX_NODE], endpoint[MAX_EDGE], nextEdge[MAX_EDGE], edgeIndex = 0;

void addUndirectedEdge(int u, int v) {
    endpoint[edgeIndex] = v;
    nextEdge[edgeIndex] = head[u];
    head[u] = edgeIndex++;

    endpoint[edgeIndex] = u;
    nextEdge[edgeIndex] = head[v];
    head[v] = edgeIndex++;

    edges[edgeIndex >> 1] = GraphEdge(u, v);
}

void resetBridgeStatus() {
    for (int i = 1; i <= edgeCount; ++i) {
        edges[i].isBridge = false;
    }
    bridgeCount = 0;
}

bool visited[MAX_NODE];

// 用于判断在去除指定边的情况下，两个顶点是否仍然连通
bool isConnectedExcludingEdge(int src, int dest, int excludeEdgeIdx) {
    memset(visited, false, sizeof(visited));
    queue<int> bfsQueue;
    visited[src] = true;

    for (int i = head[src]; i != -1; i = nextEdge[i]) {
        if ((i >> 1) != (excludeEdgeIdx - 1)) {
            bfsQueue.push(endpoint[i]);
            visited[endpoint[i]] = true;
        }
    }

    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        for (int i = head[current]; i != -1; i = nextEdge[i]) {
            int neighbor = endpoint[i];
            if (neighbor == dest) return true;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                bfsQueue.push(neighbor);
            }
        }
    }

    return false;
}

// 枚举法找割边
double baselineBridgeDetection() {
    auto tStart = high_resolution_clock::now();

    for (int i = 1; i <= edgeCount; ++i) {
        int u = edges[i].from;
        int v = edges[i].to;

        if (!isConnectedExcludingEdge(u, v, i)) {
            edges[i].isBridge = true;
            bridgeCount++;
        }
    }

    auto tEnd = high_resolution_clock::now();
    return duration_cast<duration<double, milli>>(tEnd - tStart).count();
}

void showResult() {
    printf("桥的数量是: %d\n", bridgeCount);
    /*
    for (int i = 1; i <= edgeCount; ++i) {
        if (edges[i].isBridge) {
            printf("割边：%d -> %d\n", edges[i].from, edges[i].to);
        }
    }
    */
}

int main() {
    freopen("output3.txt", "r", stdin);
    scanf("%d%d", &vertexCount, &edgeCount);

    memset(head, -1, sizeof(head));

    for (int i = 1; i <= edgeCount; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        addUndirectedEdge(u, v);
    }

    resetBridgeStatus();

    printf("基准桥边检测算法运行中...\n");
    double execTime = baselineBridgeDetection();
    printf("运行耗时：%.4fms\n", execTime);

    showResult();
    return 0;
}
