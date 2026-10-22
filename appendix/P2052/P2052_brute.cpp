// 题目：P2052 [NOI2011] 道路修建
// 暴力：逐条删边，用 BFS 数出两边的点数，累加费用
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};

int n;
int U[mxn],V[mxn],W[mxn];
vector<int>graph[mxn];
bool vis[mxn];

int bfs(int s,int banU,int banV)
{
	int cnt=0;
	queue<int>q;
	q.push(s);
	vis[s]=true;
	while(!q.empty())
	{
		int u=q.front();
		q.pop();
		++cnt;
		for(int v:graph[u])
		{
			if(vis[v]) continue;
			if((u==banU&&v==banV)||(u==banV&&v==banU)) continue;
			vis[v]=true;
			q.push(v);
		}
	}
	return cnt;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<n;++i)
	{
		cin>>U[i]>>V[i]>>W[i];
		--U[i];
		--V[i];
		graph[U[i]].push_back(V[i]);
		graph[V[i]].push_back(U[i]);
	}
	i64 ans=0;
	for(int i=1;i<n;++i)
	{
		fill_n(vis,n,false);
		int s=bfs(U[i],U[i],V[i]);
		// 两块的点数分别是 s 与 n-s
		ans+=(i64)W[i]*llabs(1LL*s-(n-s));
	}
	cout<<ans<<'\n';
	return 0;
}
