#include <iostream>
#include <string>
#include <chrono>
#include <queue>
#include <cstring>
using namespace std;
using namespace std::chrono;
/*初始化部分*/
const int MAXN=3e7+10;
const int MAXM=4e7+10;
int n;//顶点数
int m;//边数  
struct Edge{//边集
    int u,v;
    bool isbridge=false;
    /**
     * @brief 边结构体的构造函数
     * @param u 边的起点，默认为0
     * @param v 边的终点，默认为0
     * @note 初始化边的两个端点u和v
     */
    Edge(int u=0,int v=0):u(u),v(v){}
};
Edge edge[MAXM>>1];
int ans=0;
//邻接表存无向图
int h[MAXN],e[MAXM],ne[MAXM],idx; //h是起点，e是终点，ne是下一条边，idx是当前边数
void AddEdge(int u,int v)
{
    e[idx]=v,ne[idx]=h[u],h[u]=idx++; //把这条边加到u的表头
    e[idx]=u,ne[idx]=h[v],h[v]=idx++; //把这条边加到v的表头
    //edge[idx/2]=Edge(u,v);
}

void Init()
{
    for(int i=1;i<=m;i++){
        edge[i].isbridge=false;
    }
    ans=0;
}
/*初始化部分*/
/*并查集*/
struct Node{
    int u,v;
    int rk;//秩
    Node(int u=0,int v=0,int rk=0):u(u),v(v),rk(rk){}
};
Node temp[MAXM>>1];
int sp=0; //栈顶指针
/*Tarjan算法*/
int dfn[MAXN],low[MAXN];
int scc; //强连通分量
void Tarjan(int x,int pre)
{
    dfn[x]=low[x]=++scc;
    for(int i=h[x];~i;i=ne[i])//遍历邻接表
    {
        int v=e[i];
        if(!dfn[v])
        {
            Tarjan(v,i);
            low[x]=min(low[x],low[v]);
            if(dfn[x]<low[v])
            {
                edge[i/2+1].isbridge=true;
                ans++;
            }
        }else if(i!=(pre^1))
            low[x]=min(low[x],dfn[v]);
    }
}
double Tarjan_way()
{
    for(int i=1;i<=n;i++)
    {
        dfn[i]=0,low[i]=0;
    }
    scc=0;
    auto start=high_resolution_clock::now();
    for(int i=1;i<=n;i++)
    {
        if(!dfn[i])
        {
            Tarjan(i,-1);
        }
    }
    auto stop=high_resolution_clock::now();
    return duration_cast<duration<double, milli>>(stop - start).count();
}
void display()
{
    printf("桥的数量是:%d\n",ans);
    for(int i=1;i<=m;i++)
    {
        if(edge[i].isbridge)
            printf("编号为：%d %d\n",edge[i].u,edge[i].v);
    }
    for(int i=1;i<=m;i++){
        cout<<i<<" "<<dfn[i]<<" "<<low[i]<<endl;
    }
}
/*Tarjan算法*/
int main(){
    //freopen("8e6_16e6.txt", "r", stdin);
    scanf("%d%d", &n, &m);
    memset(h,-1,sizeof(h));
    for (int i = 1; i <= m; i++) 
    {
        int u, v;
        scanf("%d%d", &u, &v);
        edge[i] = Edge(u, v); // 用编号 i 记录原始边
        AddEdge(u, v);        // 建邻接表
    }
    Init();
    double time=0.0;
    printf("Tarjan算法：\n");
    time=Tarjan_way();
    printf("用时：%.4fms\n",time);
    display();
    return 0;
}
