/*
求凸包表面多边形个数
*/

#include<stdio.h>
#include<algorithm>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

const int MAXN=10;
const double eps=1e-8;

struct Point
{
    double x,y,z;
    Point() {}
    Point(double xx,double yy,double zz)
    {
        x=xx;
        y=yy;
        z=zz;
    }
};
struct Face
{
    //表示凸包一个面上的三个点的编号
    int a,b,c;
    //表示该面是否属于最终凸包上的面
    bool ok;
};

//初始顶点数
int n;
//初始顶点
Point P[MAXN];
//凸包表面的三角形数
int num;
//凸包表面的三角形
Face F[8*MAXN];
//凸包表面的三角形
int g[MAXN][MAXN];

//两向量之差
Point operator -(const Point p1,const Point p2)
{
    return Point(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
}
//两向量之和
Point operator +(const Point p1,const Point p2)
{
    return Point(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
}

//叉乘
Point operator *(const Point p1,const Point p2)
{
    return Point(p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x);
}

Point operator *(const Point p,double d)
{
    return Point(p.x*d,p.y*d,p.z*d);
}

//点乘
double  operator ^(Point p1,Point p2)
{
    return (p1.x*p2.x+p1.y*p2.y+p1.z*p2.z);
}

//向量长度
double vlen(Point p)
{
    return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}
//叉乘
Point cross(const Point &a,const Point &b,const Point &c)
{
    return Point((b.y-a.y)*(c.z-a.z)-(b.z-a.z)*(c.y-a.y),
                 (b.z-a.z)*(c.x-a.x)-(b.x-a.x)*(c.z-a.z),
                 (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x)
                );
}
//三角形面积*2
double area(Point a,Point b,Point c)
{
    return vlen((b-a)*(c-a));
}
//四面体有向体积*6
double volume(Point a,Point b,Point c,Point d)
{
    return (b-a)*(c-a)^(d-a);
}
//正：点和面朝同个方向，用体积判断
double isSameDirection(Point &p,Face &f)
{
    Point m=P[f.b]-P[f.a];
    Point n=P[f.c]-P[f.a];
    Point t=p-P[f.a];
    return (m*n)^t;
}
void solve(int p,int a,int b);//声明
void dfsFace(int p,int now)//递归搜索所有应该从凸包内删除的面
{
    F[now].ok=0;
    solve(p,F[now].b,F[now].a);
    solve(p,F[now].c,F[now].b);
    solve(p,F[now].a,F[now].c);
}
void solve(int p,int a,int b)
{
    int f=g[a][b];//搜索与该边相邻的另一个平面
    Face fa;
    if(F[f].ok)
    {
        if(isSameDirection(P[p],F[f])>eps)
            dfsFace(p,f);
        else
        {
            fa.a=b;
            fa.b=a;
            fa.c=p;//这里注意顺序，要成右手系
            fa.ok=true;
            g[p][b]=g[a][p]=g[b][a]=num;
            F[num++]=fa;
        }
    }
}
//判断两三角形是否共面
bool isCoplane(int s,int t)
{
    Point &a=P[F[s].a];
    Point &b=P[F[s].b];
    Point &c=P[F[s].c];
    return fabs(volume(a,b,c,P[F[t].a]))<eps &&
           fabs(volume(a,b,c,P[F[t].b]))<eps &&
           fabs(volume(a,b,c,P[F[t].c]))<eps;
}
//构建三维凸包
void create()
{
    int i,j,tmp;
    Face fa;

    num=0;
    if(n<4)return;
//**********************************************
    //此段是为了保证前四个点不共面
    bool flag=true;
    for(i=1; i<n; i++)
    {
        if(vlen(P[0]-P[i])>eps)
        {
            swap(P[1],P[i]);
            flag=false;
            break;
        }
    }
    if(flag)return;
    flag=true;
    //使前三个点不共线
    for(i=2; i<n; i++)
    {
        if(vlen((P[0]-P[1])*(P[1]-P[i]))>eps)
        {
            swap(P[2],P[i]);
            flag=false;
            break;
        }
    }
    if(flag)return;
    flag=true;
    //使前四个点不共面
    for(int i=3; i<n; i++)
    {
        if(fabs((P[0]-P[1])*(P[1]-P[2])^(P[0]-P[i]))>eps)//共面体积为0
        {
            swap(P[3],P[i]);
            flag=false;
            break;
        }
    }
    if(flag)return;
//*****************************************
    for(i=0; i<4; i++)
    {
        fa.a=(i+1)%4;
        fa.b=(i+2)%4;
        fa.c=(i+3)%4;
        fa.ok=true;
        if(isSameDirection(P[i],fa)>0) swap(fa.b,fa.c);
        g[fa.a][fa.b]=g[fa.b][fa.c]=g[fa.c][fa.a]=num;
        F[num++]=fa;
    }
    for(i=4; i<n; i++)
    {
        for(j=0; j<num; j++)
        {
            if(F[j].ok&&isSameDirection(P[i],F[j])>eps)
            {
                dfsFace(i,j);
                break;
            }
        }
    }
    tmp=num;
    for(i=num=0; i<tmp; i++)
        if(F[i].ok)
            F[num++]=F[i];

}

//表面多边形个数
int polygonNum()
{
    int i,j,res,flag;
    for(i=res=0; i<num; i++)
    {
        flag=1;
        for(j=0; j<i; j++)
            if(isCoplane(i,j))
            {
                flag=0;
                break;
            }
        res+=flag;
    }
    return res;
}

void init(int n)
{
    for(int i=0; i<n; i++)
    {
        scanf("%lf%lf%lf",&P[i].x,&P[i].y,&P[i].z);
    }
}
int main()
{
    freopen("in.txt","r",stdin);
    while(scanf("%d",&n)!=EOF)
    {
        init(n);
        create();
        printf("%d\n",polygonNum());
    }
    return 0;
}
