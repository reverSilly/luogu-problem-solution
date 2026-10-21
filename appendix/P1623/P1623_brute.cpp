//
// 题目：P1623 [CEOI 2007] 树的匹配 Treasury （暴力对拍用）
// 枚举所有边子集，筛出匹配，统计最大匹配与方案数
#include<bits/stdc++.h>
using namespace std;

int n;
vector<pair<int,int>>E;
int son[15][15],sn[15],hasfa[15];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
	{
		int u,m;
		cin>>u>>m;--u;
		sn[u]=m;
		for(int j=0;j<m;++j)
		{
			cin>>son[u][j];--son[u][j];
			hasfa[son[u][j]]=1;
			E.push_back({u,son[u][j]});
		}
	}
	int m=E.size();
	int mx=-1;
	long long cnt=0;
	for(int S=0;S<(1<<m);++S)
	{
		int used=0,ok=1,cc=0;
		for(int i=0;i<m;++i)
			if(S>>i&1)
			{
				auto[u,v]=E[i];
				if((used>>u&1)||(used>>v&1))
				{
					ok=0;
					break;
				}
				used|=1<<u;
				used|=1<<v;
				++cc;
			}
		if(!ok)
			continue;
		if(cc>mx)
		{
			mx=cc;
			cnt=1;
		}
		else if(cc==mx)
			++cnt;
	}
	cout<<mx<<'\n'<<cnt<<'\n';
	return 0;
}
