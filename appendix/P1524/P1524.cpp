//
// 题目：P1524 十字绣
// https://www.luogu.com.cn/problem/P1524
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{205*205};       // 顶点数 (N+1)*(M+1) <= 201*201 = 40401 (+松弛)

int n,m;
int f[mxv];                       // 并查集父节点
int deg[mxv][2];                  // deg[u][0]=正面度数, deg[u][1]=反面度数
bool hasEdge[mxv];                // 连通块是否含至少一条绣线
int diffSum[mxv];                 // 连通块内 |deg[u][0] - deg[u][1]| 之和

int id(int r,int c){return r*(m+1)+c;}
int getrt(int x){return f[x]==x?x:f[x]=getrt(f[x]);}

void addSeg(int u,int v,int side)
{
	++deg[u][side];
	++deg[v][side];
	int ru=getrt(u),rv=getrt(v);
	if(ru!=rv)
	{
		f[ru]=rv;
		hasEdge[rv]=true;
	}
	else
		hasEdge[ru]=true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	int tot=(n+1)*(m+1);
	iota(f,f+tot,0);

	for(int side=0;side<2;++side)
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
			{
				char ch;
				cin>>ch;
				if(ch=='/'||ch=='X')
					addSeg(id(i,j+1),id(i+1,j),side);
				if(ch=='\\'||ch=='X')
					addSeg(id(i,j),id(i+1,j+1),side);
			}

	for(int u=0;u<tot;++u)
		if(deg[u][0]+deg[u][1]>0)
			diffSum[getrt(u)]+=abs(deg[u][0]-deg[u][1]);

	int ans=0;
	for(int u=0;u<tot;++u)
		if(f[u]==u&&hasEdge[u])
			ans+=max(1,diffSum[u]/2);

	cout<<ans<<'\n';
	return 0;
}
