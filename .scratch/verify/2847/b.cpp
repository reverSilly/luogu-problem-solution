// P2847 暴力：候选 X = 所有平方距离排序去重，逐个 BFS 判连通
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,x[15],y[15];
i64 d2(int u,int v){i64 dx=x[u]-x[v],dy=y[u]-y[v];return dx*dx+dy*dy;}
bool conn(i64 X)
{
	bool vis[15]={};
	int q[15],h=0,t=0;
	q[t++]=0;vis[0]=1;
	while(h<t)
	{
		int u=q[h++];
		for(int v=0;v<n;++v)
			if(!vis[v]&&d2(u,v)<=X){vis[v]=1;q[t++]=v;}
	}
	return t==n;
}
int main()
{
	cin>>n;
	for(int i=0;i<n;++i)cin>>x[i]>>y[i];
	vector<i64>cand{0};
	for(int i=0;i<n;++i)for(int j=i+1;j<n;++j)cand.push_back(d2(i,j));
	sort(cand.begin(),cand.end());
	cand.erase(unique(cand.begin(),cand.end()),cand.end());
	for(i64 X:cand)if(conn(X)){cout<<X<<'\n';return 0;}
	return 0;
}