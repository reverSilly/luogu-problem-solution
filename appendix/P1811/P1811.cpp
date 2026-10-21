//
// 题目：P1811 最短路
// https://www.luogu.com.cn/problem/P1811
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{3005};

struct hsh
{
	size_t operator()(i64 x)const
	{
		uint64_t z{(uint64_t)x};
		z^=z>>30;
		z*=0xbf58476d1ce4e5b9ULL;
		z^=z>>27;
		z*=0x94d049bb133111ebULL;
		z^=z>>31;
		return (size_t)z;
	}
};

int n,m,k;
vector<int> g[mxn];
unordered_set<i64,hsh> ban,vis;
unordered_map<i64,int,hsh> from,distmp;

i64 enc(int pre,int u)
{
	return (i64)(pre+1)*mxn+u;
}

i64 tri(int a,int b,int c)
{
	return ((i64)a*mxn+b)*mxn+c;
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
	ban.reserve((size_t)k*2+1);
	for(int i=0;i<k;++i)
	{
		int a,b,c;
		cin>>a>>b>>c;
		--a;
		--b;
		--c;
		ban.insert(tri(a,b,c));
	}
	if(n==1)
	{
		cout<<"0\n1\n";
		return 0;
	}
	queue<pair<int,int>> q;
	int spre{-1},su{0};
	vis.insert(enc(spre,su));
	distmp[enc(spre,su)]=0;
	q.push({spre,su});
	bool got{false};
	int endpre{-1};
	int enddist{0};
	while(!q.empty()&&!got)
	{
		int pre{q.front().first};
		int u{q.front().second};
		q.pop();
		for(int v:g[u])
		{
			if(pre!=-1&&ban.count(tri(pre,u,v)))
				continue;
			i64 nk{enc(u,v)};
			if(vis.count(nk))
				continue;
			vis.insert(nk);
			from[nk]=pre;
			distmp[nk]=distmp[enc(pre,u)]+1;
			if(v==n-1)
			{
				got=true;
				endpre=u;
				enddist=distmp[nk];
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
		int ppre{from[enc(pre,cur)]};
		cur=pre;
		pre=ppre;
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
