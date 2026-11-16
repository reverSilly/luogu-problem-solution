// 题目：P2752 [USACO4.3] 街道赛跑Street Race
// 不可避免点：删去 S 后 0 不再能到达 N。
// 中间路口：S 不可避免，且记 A 为 G-S 中 0 可达点集，
//           不存在 u∉A → v∈A 的边（这种边无法划分给任何一半），且 S 无自环。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};

int n;
vector<int>graph[mxn];
bool vis[mxn];

void bfs(int s,int ban)	// vis = 从 s 出发、不经过 ban 的可达点集；ban=-1 表示无禁点
{
	fill_n(vis,n+1,false);
	queue<int>Q;
	vis[s]=true;Q.push(s);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(v!=ban&&!vis[v])
			{
				vis[v]=true;
				Q.push(v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int x,cur=0;
	while(cin>>x&&x!=-1)
	{
		if(x==-2)
			++cur;	// 一个邻接表结束；cur 即当前路口编号
		else
			graph[cur].push_back(x);
	}
	n=cur-1;	// 共 N+1 张邻接表（路口 0..N），最后一行是 -1
	vector<int>unav,mid;
	for(int s=1;s<n;++s)
	{
		bfs(0,s);
		if(vis[n])
			continue;	// 删 s 后 0 仍可达 N ⇒ 不是不可避免点
		unav.push_back(s);
		bool ok=true;
		for(int v:graph[s])
			if(v==s)
				ok=false;	// S 自环只能归第一半，但终点不许有出边
		for(int u=0;u<=n&&ok;++u)
			for(int v:graph[u])
				if(!vis[u]&&vis[v])
					ok=false;	// 从 A 外指进 A 的边：两半都放不下
		if(ok)
			mid.push_back(s);
	}
	cout<<unav.size();
	for(int v:unav)
		cout<<' '<<v;
	cout<<'\n'<<mid.size();
	for(int v:mid)
		cout<<' '<<v;
	cout<<'\n';
	return 0;
}