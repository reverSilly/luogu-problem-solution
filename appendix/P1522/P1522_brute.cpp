//
// 题目：P1522 [USACO2.4] 牛的旅行 Cow Tours
// 暴力基准: 枚举每一对不同连通块顶点 (u,v), 实打实加边后重跑一次 Floyd 求合并后连通块的真实直径
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{45};
constexpr double inf{1e18};

int n;
double px[mxn],py[mxn];
double g[mxn][mxn],d[mxn][mxn];

double euc(int i,int j)
{
	return hypot(px[i]-px[j],py[i]-py[j]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];

	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			char ch;
			cin>>ch;
			if(i==j)
				g[i][j]=0;
			else if(ch=='1')
				g[i][j]=euc(i,j);
			else
				g[i][j]=inf;
			d[i][j]=g[i][j];
		}

	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				d[i][j]=min(d[i][j],d[i][k]+d[k][j]);

	double ans=inf;
	for(int u=0;u<n;++u)
		for(int v=u+1;v<n;++v)
			if(d[u][v]>inf/2)
			{
				double w=euc(u,v);
				double cur=0;
				for(int a=0;a<n;++a)
					if(d[a][u]<inf/2||d[a][v]<inf/2)
						for(int b=a+1;b<n;++b)
							if(d[b][u]<inf/2||d[b][v]<inf/2)
							{
								double dist_ab=min({d[a][b],d[a][u]+w+d[v][b],d[a][v]+w+d[u][b]});
								cur=max(cur,dist_ab);
							}
				ans=min(ans,cur);
			}

	cout<<fixed<<setprecision(6)<<ans<<'\n';
	return 0;
}
