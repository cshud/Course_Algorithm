#include<stdio.h>
#include<string.h>
#include<iostream>
#include<vector>
#define MAXX 20004
using namespace std;
int n,m,x,y,cot;
int low[MAXX],dfn[MAXX],parent[MAXX];
bool vis[MAXX],flag[MAXX];
vector<int> v[MAXX];
void dfs(int index)
{
    vis[index]=true;
    low[index]=dfn[index]=++cot;
    int child=0;
    for(int i=0; i<v[index].size(); i++)
    {
        if(vis[v[index][i]]==false)
        {
            child++;
            parent[ v[index][i] ]=index;
            dfs(v[index][i]);
            low[index]=min( low[index],low[v[index][i]] );
            if(parent[index]==-1 && child>1 && flag[index]==false)
            {
                printf("%d\n",index);
                flag[index]=true;
            }
            if(parent[index]!=-1 && low[ v[index][i] ]>=dfn[index] && flag[index]==false)
            {
                printf("%d\n",index);
                flag[index]=true;
            }
        }
        else if(v[index][i]!=parent[index])
            low[index]=min(low[index],dfn[ v[index][i] ]);
    }
}
int main()
{
    freopen("data1.txt","r",stdin);
    freopen("out.txt","w",stdout);
    cot=0;
    n=0;
  while(scanf("%*c%d%*c%d%*c",&x,&y)!=EOF)   //注意输入的数据格式
   // while(scanf("%d%d",&x,&y)!=EOF)
    {
        v[x].push_back(y);
        v[y].push_back(x);
        n=max(n,x);
        n=max(n,y);
    }
    memset(vis,false,sizeof(bool)*(n+1));
    memset(parent,-1,sizeof(int)*(n+1));
    memset(flag,false,sizeof(bool)*(n+1));
    dfs(1);
    return 0;
}
