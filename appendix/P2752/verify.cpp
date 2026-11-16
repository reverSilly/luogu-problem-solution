// P2752 轻量验证（性质隐蔽："中间路口"的刻画并不显然）
// fast：题解所用刻画 —— S 不可避免，且记 A 为 G-S 中 0 可达点集，
//       不存在 u∉A → v∈A 的边，且 S 无自环。
// brute：枚举 V\{S} 的全部划分 V1⊔V2（0∈V1，N∈V2），要求无跨越边、无 S 自环，
//        且诱导子图 G[V1∪{S}]（起 0 终 S）与 G[V2∪{S}]（起 S 终 N）都是"良好跑道"。
// 随机生成小型良好跑道（路口数 n+1≤7）逐项对比两种刻画。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10};

int n;
vector<int>graph[mxn];
bool vis[mxn];

void bfs(int s,int ban)
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

bool fastmid(int s)
{
	bfs(0,s);
	if(vis[n])
		return false;
	for(int v:graph[s])
		if(v==s)
			return false;
	for(int u=0;u<=n;++u)
		for(int v:graph[u])
			if(!vis[u]&&vis[v])
				return false;
	return true;
}

bool inset(const vector<int>&V,int v)
{
	return find(V.begin(),V.end(),v)!=V.end();
}

bool good(int s,int t,const vector<int>&V)	// G[V]：s 可达全体、全体可达 t、t 在 V 内无出边
{
	for(int v:graph[t])
		if(inset(V,v))
			return false;
	fill_n(vis,n+1,false);
	queue<int>Q;
	vis[s]=true;Q.push(s);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(inset(V,v)&&!vis[v])
			{
				vis[v]=true;
				Q.push(v);
			}
	}
	for(int v:V)
		if(!vis[v])
			return false;
	fill_n(vis,n+1,false);
	vis[t]=true;Q.push(t);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int p=0;p<=n;++p)
			if(inset(V,p)&&!vis[p])
				for(int v:graph[p])
					if(v==u)
					{
						vis[p]=true;
						Q.push(p);
						break;
					}
	}
	for(int v:V)
		if(!vis[v])
			return false;
	return true;
}

bool brutemid(int s)
{
	vector<int>rest;
	for(int v=0;v<=n;++v)
		if(v!=s)
			rest.push_back(v);
	int k=rest.size();
	for(int mask=0;mask<(1<<k);++mask)
	{
		vector<int>V1{s},V2{s};
		for(int i=0;i<k;++i)
			(mask>>i&1?V1:V2).push_back(rest[i]);
		if(!inset(V1,0)||!inset(V2,n))
			continue;
		bool ok=true;
		for(int u=0;u<=n&&ok;++u)
			for(int v:graph[u])
			{
				if(u==s&&v==s)
					ok=false;	// 自环会成为两半的公共街道
				if(u!=s&&v!=s&&inset(V1,u)!=inset(V1,v))
					ok=false;	// 跨越边无法划分
			}
		if(ok&&good(0,s,V1)&&good(s,n,V2))
			return true;
	}
	return false;
}

mt19937 rn(20260601);

bool gen()	// 拒绝采样：随机图 + 检验"良好跑道"三条
{
	n=2+rn()%5;	// 终点编号 2..6
	for(int i=0;i<=n;++i)
		graph[i].clear();
	for(int u=0;u<n;++u)	// 终点 n 无出边
		for(int v=0;v<=n;++v)
			if(u!=v&&rn()%100<30)
				graph[u].push_back(v);
	bfs(0,-1);
	for(int v=0;v<=n;++v)
		if(!vis[v])
			return false;
	for(int s=0;s<n;++s)
	{
		bfs(s,-1);
		if(!vis[n])
			return false;
	}
	return true;
}

void dump()
{
	printf("n=%d\n",n);
	for(int u=0;u<=n;++u)
	{
		printf("%d:",u);
		for(int v:graph[u])
			printf(" %d",v);
		printf("\n");
	}
}

int main()
{
	int cnt=0;
	for(int it=0;it<500000&&cnt<2000;++it)
	{
		if(!gen())
			continue;
		++cnt;
		for(int s=1;s<n;++s)
			if(fastmid(s)!=brutemid(s))
			{
				printf("MISMATCH n=%d s=%d\n",n,s);
				dump();
				return 1;
			}
	}
	printf("%d random good courses: splitting-point characterization agrees with partition brute force\n",cnt);
	return 0;
}