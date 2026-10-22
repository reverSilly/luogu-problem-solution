// P2196 暴力：DFS 枚举所有合法路径，求最大地雷数与字典序最小的路径
#include<bits/stdc++.h>
using namespace std;

int n;
int a[25];
bool g[25][25];
int best=-1;
vector<int> bestPath;

void dfs(int u,vector<int>&path,int sum)
{
	path.push_back(u);
	if(sum>best||(sum==best&&path<bestPath))
	{
		best=sum;
		bestPath=path;
	}
	for(int v=u+1;v<=n;++v)
		if(g[u][v])dfs(v,path,sum+a[v]);
	path.pop_back();
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)cin>>a[i];
	for(int i=1;i<n;++i)
		for(int j=i+1;j<=n;++j)
		{
			int x;cin>>x;
			g[i][j]=x;
		}
	for(int s=1;s<=n;++s)
	{
		vector<int> path;
		dfs(s,path,a[s]);
	}
	for(size_t i=0;i<bestPath.size();++i)
	{
		if(i)cout<<' ';
		cout<<bestPath[i];
	}
	cout<<'\n'<<best<<'\n';
	return 0;
}
