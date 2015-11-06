/*
��͹���������θ���
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
    //��ʾ͹��һ�����ϵ�������ı��
    int a,b,c;
    //��ʾ�����Ƿ���������͹���ϵ���
    bool ok;
};

//��ʼ������
int n;
//��ʼ����
Point P[MAXN];
//͹���������������
int num;
//͹�������������
Face F[8*MAXN];
//͹�������������
int g[MAXN][MAXN];

//������֮��
Point operator -(const Point p1,const Point p2)
{
    return Point(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
}
//������֮��
Point operator +(const Point p1,const Point p2)
{
    return Point(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
}

//���
Point operator *(const Point p1,const Point p2)
{
    return Point(p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x);
}

Point operator *(const Point p,double d)
{
    return Point(p.x*d,p.y*d,p.z*d);
}

//���
double  operator ^(Point p1,Point p2)
{
    return (p1.x*p2.x+p1.y*p2.y+p1.z*p2.z);
}

//��������
double vlen(Point p)
{
    return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}
//���
Point cross(const Point &a,const Point &b,const Point &c)
{
    return Point((b.y-a.y)*(c.z-a.z)-(b.z-a.z)*(c.y-a.y),
                 (b.z-a.z)*(c.x-a.x)-(b.x-a.x)*(c.z-a.z),
                 (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x)
                );
}
//���������*2
double area(Point a,Point b,Point c)
{
    return vlen((b-a)*(c-a));
}
//�������������*6
double volume(Point a,Point b,Point c,Point d)
{
    return (b-a)*(c-a)^(d-a);
}
//��������泯ͬ������������ж�
double isSameDirection(Point &p,Face &f)
{
    Point m=P[f.b]-P[f.a];
    Point n=P[f.c]-P[f.a];
    Point t=p-P[f.a];
    return (m*n)^t;
}
void solve(int p,int a,int b);//����
void dfsFace(int p,int now)//�ݹ���������Ӧ�ô�͹����ɾ������
{
    F[now].ok=0;
    solve(p,F[now].b,F[now].a);
    solve(p,F[now].c,F[now].b);
    solve(p,F[now].a,F[now].c);
}
void solve(int p,int a,int b)
{
    int f=g[a][b];//������ñ����ڵ���һ��ƽ��
    Face fa;
    if(F[f].ok)
    {
        if(isSameDirection(P[p],F[f])>eps)
            dfsFace(p,f);
        else
        {
            fa.a=b;
            fa.b=a;
            fa.c=p;//����ע��˳��Ҫ������ϵ
            fa.ok=true;
            g[p][b]=g[a][p]=g[b][a]=num;
            F[num++]=fa;
        }
    }
}
//�ж����������Ƿ���
bool isCoplane(int s,int t)
{
    Point &a=P[F[s].a];
    Point &b=P[F[s].b];
    Point &c=P[F[s].c];
    return fabs(volume(a,b,c,P[F[t].a]))<eps &&
           fabs(volume(a,b,c,P[F[t].b]))<eps &&
           fabs(volume(a,b,c,P[F[t].c]))<eps;
}
//������ά͹��
void create()
{
    int i,j,tmp;
    Face fa;

    num=0;
    if(n<4)return;
//**********************************************
    //�˶���Ϊ�˱�֤ǰ�ĸ��㲻����
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
    //ʹǰ�����㲻����
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
    //ʹǰ�ĸ��㲻����
    for(int i=3; i<n; i++)
    {
        if(fabs((P[0]-P[1])*(P[1]-P[2])^(P[0]-P[i]))>eps)//�������Ϊ0
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

//�������θ���
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
