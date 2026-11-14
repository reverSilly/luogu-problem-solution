#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m;						// n 个路口 0..n-1，起点 0，终点 n-1
vector<int>graph[mxn];
bool A[mxn],R[mxn];

// 从 s 出发 BFS，禁止经过 ban（s 自身作为起点仍可扩展），标记 vis
void bfs(int s,int ban,bool*vis)
{
	for(int i=0;i<n;++i)
		vis[i]=false;
	queue<int>Q;
	vis[s]=true;Q.push(s);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(v!=ban&&!vis[v])
				vis[v]=true,Q.push(v);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int cur=0,x;
	while(cin>>x)
	{
		if(x==-1)
			break;
		if(x==-2)
			++cur;
		else
			graph[cur].push_back(x),++m;
	}
	n=cur;						// -2 的个数 = N+1 = 路口数
	vector<int>unav;
	for(int v=1;v<n-1;++v)		// 第一问：删掉 v 后 0 到不了终点 ⇒ 不可避免
	{
		bfs(0,v,A);
		if(!A[n-1])
			unav.push_back(v);
	}
	cout<<unav.size();
	for(int v:unav)
		cout<<' '<<v;
	cout<<'\n';
	vector<int>split;
	for(int s:unav)				// 第二问：中间路口 ⊆ 不可避免点
	{
		bfs(0,s,A);				// A = G-s 中从 0 可达（前半）
		bfs(s,s,R);				// R = G-s 中从 s 可达（后半）
		bool ok=true;
		for(int v=0;v<n&&ok;++v)
			if(v!=s&&!(A[v]^R[v]))	// 每个非 s 点恰好属于一侧
				ok=false;
		for(int u=0;u<n&&ok;++u)
			for(int v:graph[u])
			{
				if(u==s)			// 禁止 s->A（s 的出边只能进后半）
				{
					if(A[v])
						ok=false;
				}
				else if(v!=s&&A[u]!=A[v])	// 禁止 A<->R 跨边
					ok=false;
				if(!ok)
					break;
			}
		if(ok)
			split.push_back(s);
	}
	cout<<split.size();
	for(int v:split)
		cout<<' '<<v;
	cout<<'\n';
	return 0;
}