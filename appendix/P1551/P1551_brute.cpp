// 题目：P1551 亲戚
// 暴力基准：对每个询问做一次 BFS,直接在关系图上判可达
// 仅用于对拍,不能提交
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,m,p;
vector<int>graph[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p;
	for(int i=0,a,b;i<m;++i)
	{
		cin>>a>>b;--a;--b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}
	for(int i=0,a,b;i<p;++i)
	{
		cin>>a>>b;--a;--b;
		fill_n(vis,n,false);
		queue<int>Q;
		Q.push(a);
		vis[a]=true;
		while(!Q.empty())
		{
			int u=Q.front();Q.pop();
			for(int v:graph[u])
				if(!vis[v])
				{
					vis[v]=true;
					Q.push(v);
				}
		}
		cout<<(vis[b]?"Yes\n":"No\n");
	}
	return 0;
}
