#include<bits/stdc++.h>
using namespace std;

constexpr int nn{52};
constexpr int inf{1145141919};
constexpr int mxp{10005};

int P,cnt;
struct{int u,v,w;}E[mxp*2];       // 无向边存两遍

int id(char c)
{
	if(c>='a'&&c<='z')
		return c-'a';
	if(c>='A'&&c<='Y')
		return c-'A'+26;
	return 51;
}

int dis[nn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>P;
	for(int i=0,a,b,w;i<P;++i)
	{
		char x,y;
		cin>>x>>y>>w;
		a=id(x);b=id(y);
		E[cnt++]={a,b,w};
		E[cnt++]={b,a,w};
	}
	fill_n(&dis[0],nn,inf);
	dis[51]=0;                     // 从谷仓出发做 Bellman-Ford
	for(int it=0;it<nn-1;++it)
		for(int i=0;i<cnt;++i)
			if(dis[E[i].u]<inf&&dis[E[i].u]+E[i].w<dis[E[i].v])
				dis[E[i].v]=dis[E[i].u]+E[i].w;
	int best=inf,bi=-1;
	for(int i=26;i<=50;++i)
		if(dis[i]<best)
		{
			best=dis[i];
			bi=i;
		}
	cout<<char('A'+bi-26)<<' '<<best<<'\n';
	return 0;
}
