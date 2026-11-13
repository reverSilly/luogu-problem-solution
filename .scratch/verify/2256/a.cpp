// 题目：P2256 一中校运会之百米跑
// 思路：同组 = 无向图连通；边读边用并查集合并
//       名字用 unordered_map 映射为 0..n-1
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int n,m,k,f[mxn],sz[mxn];
unordered_map<string,int> id;

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

void unite(int u,int v)
{
	int ru=getrt(u),rv=getrt(v);
	if(ru==rv)
		return;
	if(sz[ru]<sz[rv])
		swap(ru,rv);
	f[rv]=ru;
	sz[ru]+=sz[rv];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	id.reserve(n*2);
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		id[s]=i;
		f[i]=i;
		sz[i]=1;
	}
	for(int i=0,u,v;i<m;++i)
	{
		string a,b;
		cin>>a>>b;
		u=id[a];v=id[b];
		unite(u,v);
	}
	cin>>k;
	for(int i=0;i<k;++i)
	{
		string a,b;
		cin>>a>>b;
		cout<<(getrt(id[a])==getrt(id[b])?"Yes.\n":"No.\n");
	}
	return 0;
}
