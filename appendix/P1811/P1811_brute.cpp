//
// 题目：P1811 最短路
// https://www.luogu.com.cn/problem/P1811
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{12};

int n,m,k;
vector<int> g[mxn];
vector<i64> ban;
int dist[mxn+1][mxn];
int ppre[mxn+1][mxn];
bool vis[mxn+1][mxn];

i64 tri(int a,int b,int c)
{
	return ((i64)a*mxn+b)*mxn+c;
}

bool blocked(int a,int b,int c)
{
	return binary_search(ban.begin(),ban.end(),tri(a,b,c));
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=0;i<m;++i)
	{
		int u,v;
		cin>>u>>v;
		--u;
		--v;
		g[u].push_back(v);
		if(u!=v)
			g[v].push_back(u);
	}
	for(int i=0;i<k;++i)
	{
		int a,b,c;
		cin>>a>>b>>c;
		--a;
		--b;
		--c;
		ban.push_back(tri(a,b,c));
	}
	sort(ban.begin(),ban.end());
	if(n==1)
	{
		cout<<"0\n1\n";
		return 0;
	}
	queue<pair<int,int>> q;
	int spre{n};
	int su{0};
	vis[spre][su]=true;
	dist[spre][su]=0;
	ppre[spre][su]=-1;
	q.push({spre,su});
	bool got{false};
	int endpre{-1};
	int enddist{0};
	while(!q.empty()&&!got)
	{
		int pre{q.front().first};
		int u{q.front().second};
		q.pop();
		int realpre{pre==n?-1:pre};
		for(int v:g[u])
		{
			if(realpre!=-1&&blocked(realpre,u,v))
				continue;
			if(vis[u][v])
				continue;
			vis[u][v]=true;
			dist[u][v]=dist[pre][u]+1;
			ppre[u][v]=realpre;
			if(v==n-1)
			{
				got=true;
				endpre=u;
				enddist=dist[u][v];
				break;
			}
			q.push({u,v});
		}
	}
	if(!got)
	{
		cout<<"-1\n";
		return 0;
	}
	vector<int> seq;
	int cur{n-1};
	int pre{endpre};
	seq.push_back(cur);
	while(pre!=-1)
	{
		seq.push_back(pre);
		int pp{ppre[pre][cur]};
		cur=pre;
		pre=pp;
	}
	reverse(seq.begin(),seq.end());
	cout<<enddist<<'\n';
	for(int i=0;i<(int)seq.size();++i)
	{
		if(i)
			cout<<' ';
		cout<<seq[i]+1;
	}
	cout<<'\n';
	return 0;
}
