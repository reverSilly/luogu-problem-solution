//
// 题目：P1882 [USACO10OPEN] 接力赛跑 B
// https://www.luogu.com.cn/problem/P1882
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int inf{1145141919};

vector<int> g[mxn];
int len[mxn],stt[mxn];
bool fired[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		int m;
		cin>>len[i]>>m;
		g[i].clear();
		while(m--)
		{
			int v;
			cin>>v;
			--v;
			g[i].push_back(v);
		}
		stt[i]=inf;
	}
	stt[0]=0;
	int ans=0;
	for(int it=0;it<n;++it)
	{
		int u=-1,best=inf;
		for(int i=0;i<n;++i)
		{
			if(fired[i]||stt[i]>=inf)
				continue;
			int ft=stt[i]+len[i];
			if(ft<best)
			{
				best=ft;
				u=i;
			}
		}
		if(u<0)
			break;
		fired[u]=true;
		ans=max(ans,best);
		for(int v:g[u])
			if(!fired[v]&&stt[v]>best)
				stt[v]=best;
	}
	cout<<ans<<'\n';
	return 0;
}
