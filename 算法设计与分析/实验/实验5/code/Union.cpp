#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_VTX = 1e6 + 10;
const int MAX_EDG = 3e7 + 10;

int totalNodes, totalEdges;

struct BridgeEdge {
    int from, to;
    bool isCritical;

    BridgeEdge(int u = 0, int v = 0) : from(u), to(v), isCritical(false) {}
};

BridgeEdge bridgeSet[MAX_EDG >> 1];
int bridgeCounter = 0;

// 图的邻接表：head -> next -> endpoint
int head[MAX_VTX], endpoint[MAX_EDG], nextEdge[MAX_EDG], edgePtr = 0;

void insertEdge(int u, int v) {
    endpoint[edgePtr] = v;
    nextEdge[edgePtr] = head[u];
    head[u] = edgePtr++;

    endpoint[edgePtr] = u;
    nextEdge[edgePtr] = head[v];
    head[v] = edgePtr++;

    bridgeSet[edgePtr >> 1] = BridgeEdge(u, v);
}

void resetBridgeStatus() {
    for (int i = 1; i <= totalEdges; ++i)
        bridgeSet[i].isCritical = false;
    bridgeCounter = 0;
}

// 并查集相关结构
struct UnionLog {
    int u, v, prevRank;
    UnionLog(int u = 0, int v = 0, int r = 0) : u(u), v(v), prevRank(r) {}
};

UnionLog history[MAX_EDG >> 1];
int top = 0;

int parent[MAX_VTX], rankVal[MAX_VTX];

int findSet(int x) {
    return parent[x] == x ? x : parent[x] = findSet(parent[x]);
}

void mergeSet(int x, int y) {
    int fx = findSet(x);
    int fy = findSet(y);
    if (fx == fy) return;

    if (rankVal[fx] < rankVal[fy]) {
        history[++top] = UnionLog(fy, fx, rankVal[fy]);
        parent[fx] = fy;
    } else {
        history[++top] = UnionLog(fx, fy, rankVal[fx]);
        parent[fy] = fx;
        if (rankVal[fx] == rankVal[fy]) rankVal[fx]++;
    }
}

void rollbackTo(int checkpoint) {
    while (top > checkpoint) {
        UnionLog entry = history[top--];
        parent[entry.v] = entry.v;
        rankVal[entry.u] = entry.prevRank;
    }
}

void evaluateBridges() {
    for (int i = 1; i <= totalEdges; ++i) {
        int u = bridgeSet[i].from;
        int v = bridgeSet[i].to;

        int prevStack = top;

        for (int j = totalEdges; j >= 1; --j) {
            if (j == i) continue;
            mergeSet(bridgeSet[j].from, bridgeSet[j].to);
        }

        if (findSet(u) != findSet(v)) {
            bridgeSet[i].isCritical = true;
            bridgeCounter++;
        }

        rollbackTo(prevStack);
    }
}

double detectBridgesUF() {
    for (int i = 1; i <= totalNodes; ++i) {
        parent[i] = i;
        rankVal[i] = 0;
    }
    top = 0;

    auto beginTime = high_resolution_clock::now();
    evaluateBridges();
    auto endTime = high_resolution_clock::now();

    return duration_cast<duration<double, milli>>(endTime - beginTime).count();
}

void printResults() {
    printf("桥的数量是: %d\n", bridgeCounter);
    /*
    for (int i = 1; i <= totalEdges; ++i) {
        if (bridgeSet[i].isCritical)
            printf("割边：%d - %d\n", bridgeSet[i].from, bridgeSet[i].to);
    }
    */
}

int main() {
    freopen("output3.txt", "r", stdin);
    scanf("%d%d", &totalNodes, &totalEdges);

    memset(head, -1, sizeof(head));

    for (int i = 1; i <= totalEdges; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        insertEdge(u, v);
    }

    resetBridgeStatus();

    printf("并查集桥边检测法执行中...\n");
    double duration = detectBridgesUF();
    printf("检测耗时: %.4fms\n", duration);

    printResults();
    return 0;
}
