#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<queue>
#include<string.h>
#include<time.h>
using namespace std;
struct Point
{
    unsigned int num;
    char ch1[5];
    char ch2[9];
    char ch3[11];
    char ch4[41];
};
struct Node
{
    Point point;
    queue<Point> q;
    Node *left,*right;
    unsigned int l,r;
    int cot;
    Node()
    {
        cot=0;
        left=right=NULL;
    }
};
Point tem;
void insert(Node &root,Point X)
{
    root.q.push(X);
}
bool deleteindex(Node *root,unsigned int x)
{
//	printf("%u %u\n",root->l,root->r);
    if(root->l==root->r)
        return -1;
    else
    {
        if(root->left==NULL)
        {
            root->left=new Node();
            root->left->l=root->l;
            root->left->r=root->l+(root->r - root->l)/2;
        }
        if(root->right==NULL)
        {
            root->right=new Node();
            root->right->l=root->l+(root->r-root->l)/2+1;
            root->right->r=root->r;
        }
    }
    if(root->q.size()!=0)
    {
        while(!root->q.empty())
        {
            tem=root->q.front();
            root->q.pop();
            if(tem.num<=(root->left->r))
            {
                root->left->q.push(tem);
                root->left->cot++;
            }
            else
            {
                root->right->q.push(tem);
                root->right->cot++;
            }
        }
    }
    bool isdelete;
    if(x<=root->left->r)
    {
        if(root->left->l==root->left->r)
        {
            delete root->left;
            root->left=NULL;
            isdelete=true;
        }
        else
            isdelete=deleteindex(root->left,x);
    }
    else
    {
        if(root->right->r==root->right->l)
        {
            delete root->right;
            root->right=NULL;
            isdelete=true;
        }
        else
            isdelete=deleteindex(root->right,x);
    }
    if(isdelete)
        root->cot--;
    return isdelete;
}
void query(Node *root,unsigned int x)
{
//	printf("%u %u\n",root->l,root->r);
    if(root->l==root->r )
    {
        if(!root->q.empty())
            printf("%d %s %s %s %s\n",root->q.front().num,root->q.front().ch1,root->q.front().ch2,root->q.front().ch3,root->q.front().ch4);
        else
            printf("没有找到\n");
        return;
    }
    else if(root->q.size()!=0)
    {
        if(root->left==NULL)
        {
            root->left=new Node();
            root->left->l=root->l;
            root->left->r=root->l+(root->r - root->l)/2;
        }
        if(root->right==NULL)
        {
            root->right=new Node();
            root->right->l=root->l+(root->r-root->l)/2+1;
            root->right->r=root->r;
        }
    }
    if(root->q.size()!=0)
    {
        while(!root->q.empty())
        {
            tem=root->q.front();
            root->q.pop();
            if(tem.num<=(root->left->r))
            {
                root->left->q.push(tem);
                root->left->cot++;
            }
            else
            {
                root->right->q.push(tem);
                root->right->cot++;
            }
        }
    }
    if(x<=root->left->r)
    {
        if(root->left!=NULL)
            query(root->left,x);
        else
            printf("没有该项\n");
    }
    else
    {
        if(root->right!=NULL)
            query(root->right,x);
        else
            printf("没有该项\n");
    }
}

int main()
{
    freopen("测试数据1.txt","r",stdin);
    freopen("out.txt","w",stdout);
    Node root;
    root.l=0;
    root.r=UINT_MAX;
    int t1,t2;
    t1=clock();
    while(scanf("%d%s%s%s%s",&tem.num,tem.ch1,tem.ch2,tem.ch3,tem.ch4)!=EOF)
    {
        insert(root,tem);
    }
    t2=clock();
    printf("输入数据用时 %d 毫秒\n",t2-t1);
    t1=clock();
    for(int i=0; i<10000; i++)
    {
        if(deleteindex(&root,535103)==1)
            printf("删除成功\n");
        else
            printf("无此节点\n");
    }
    t2=clock();
    printf("删除10000次操作平均用时 %.4lf 毫秒",(t2-t1+0.0)/10000);
    t1=clock();
    for(int i=0; i<10000; i++)
        query(&root,535103);
    t2=clock();
    printf("查询10000次操作平均用时 %.4lf 毫秒",(t2-t1+0.0)/10000);
    return 0;
}
