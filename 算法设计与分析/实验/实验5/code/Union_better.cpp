#include <iostream>
#include <string>
#include <chrono>
#include <queue>
#include <cstring>
using namespace std;
using namespace std::chrono;

const int MAXN = 3e7 + 10;
const int MAXM = 4e7 + 10;
int n, m; // 顶点数、边数

struct Edge {
    int u, v;
    bool isbridge = false;
    Edge(int u = 0, int v = 0) : u(u), v(v) {}
};

Edge edge[MAXM >> 1];
int ans = 0;

int h[MAXN], e[MAXM], ne[MAXM], idx;

void AddEdge(int u, int v) {
    e[idx] = v, ne[idx] = h[u], h[u] = idx++;
    e[idx] = u, ne[idx] = h[v], h[v] = idx++;
    // edge[i] 在主函数里分配！
}

void display() {
    printf("桥的数量:%d\n", ans);
    /*for (int i = 1; i <= m; i++) {
        if (edge[i].isbridge)
            printf("%d %d\n", edge[i].u, edge[i].v);
    }*/
}

void Init() {
    for (int i = 1; i <= m; i++) edge[i].isbridge = false;
    ans = 0;
}

struct Node {
    int u, v, rk;
    Node(int u = 0, int v = 0, int rk = 0) : u(u), v(v), rk(rk) {}
};

vector<Node> temp;
int sp = 0;
int rk[MAXN], fa[MAXN];

int find(int u) {
    while (u != fa[u]) u = fa[u];
    return u;
}

void Union(int u, int v) {
    int pu = find(u), pv = find(v);
    if (pu == pv) return;
    if (rk[pu] < rk[pv]) temp.push_back(Node(pv, pu, rk[pv]));
    else temp.push_back(Node(pu, pv, rk[pu]));
    if (rk[pu] < rk[pv]) fa[pu] = pv;
    else if (rk[pu] == rk[pv]) fa[pv] = pu, rk[pu]++;
    else fa[pv] = pu;
}

void Pop(int sz) {
    while ((int)temp.size() > sz) {
        Node t = temp.back(); temp.pop_back();
        rk[t.u] = t.rk;
        fa[t.v] = t.v;
    }
}


void Push(int l, int r) {
    for (int i = l; i <= r; i++) {
        Union(edge[i].u, edge[i].v);
    }
}

void DFS(int l, int r) {
    if (l == r) {
        if (find(edge[l].u) != find(edge[l].v)) {
            edge[l].isbridge = true;
            ans++;
        }
        return;
    }

    int siz = temp.size();
    int mid = (l + r) >> 1;

    Push(mid + 1, r); //右边入栈
    DFS(l, mid); //搜索左边
    Pop(siz);

    Push(l, mid);
    DFS(mid + 1, r);
    Pop(siz);
}

double better_way() {
    for (int i = 1; i <= n; i++) fa[i] = i, rk[i] = 0;
    sp = 0;
    auto start = high_resolution_clock::now();
    DFS(1, m);
    auto stop = high_resolution_clock::now();
    return duration_cast<duration<double, milli>>(stop - start).count();
}

int main() {
    freopen("1e6_2e6.txt", "r", stdin);
    scanf("%d%d", &n, &m);
    memset(h, -1, sizeof(h));
    for (int i = 1; i <= m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        edge[i] = Edge(u, v); // 确保边编号与输入顺序一致
        AddEdge(u, v);
    }
    Init();
    printf("并查集+线段树分治算法:\n");
    double time = better_way();
    printf("时间：%.4fms\n", time);
    display();
    return 0;
}