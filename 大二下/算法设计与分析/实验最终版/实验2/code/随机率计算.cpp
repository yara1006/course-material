#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unordered_set>
using namespace std;
struct Point{
    int x=0;
    int y=0;

    bool operator==(const Point&other) const{
        return x==other.x&&y==other.y;
    }
    // 定义哈希函数，使得 Point 能够用于 unordered_set
    struct Hash{
        size_t operator()(const Point&p)const{
            return hash<int>()(p.x)^hash<int>()(p.y);// 哈希值是 x 和 y 的哈希值的异或
        }
    };
};
double calculateDuplicateRate(int N){
    unordered_set<Point,Point::Hash>uniquePoints;
    int duplicateCount=0;

    for(int i=0;i<N;++i){
        Point p;
        p.x=rand()%10000;
        p.y=rand()%10000;
        if(uniquePoints.find(p)!=uniquePoints.end()){
            duplicateCount++;
        }else{
            uniquePoints.insert(p);
        }
    }
    return (double)duplicateCount/N;
    
}
double distance(const Point &p1,const Point &p2){
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}

int main()
{
    srand(time(0));
    vector<int>pointSizes={100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000};
    for(int N:pointSizes){
        double duplicateRate=calculateDuplicateRate(N);
        cout<<"size: "<<N<<" "<<duplicateRate*100<<"%"<<endl;
    }
    return 0;
}