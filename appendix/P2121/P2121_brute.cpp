#include<bits/stdc++.h>
using namespace std;

using i64=long long;

//暴力：枚举所有边的子集，取其中无环且不超过 K 条的权值和最大者
constexpr int mxn{25};
int n,m,K;
int eu[mxn],ev[mxn],ew[mxn];
int fa[mxn];

int getrt(int x)
{
	return fa[x]==x?x:fa[x]=getrt(fa[x]);
}

bool noCycle(const vector<int>&pick)
{
	for(int i=0;i<n;++i)fa[i]=i;
	for(int id:pick)
	{
		int u=getrt(eu[id]),v=getrt(ev[id]);
		if(u==v)return false;
		fa[u]=v;
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m>>K;
	for(int i=0;i<m;++i)
		cin>>eu[i]>>ev[i]>>ew[i];
	int ans=0;
	vector<int>pick;
	for(int s=0;s<(1<<m);++s)
	{
		pick.clear();
		for(int i=0;i<m;++i)
			if(s>>i&1)pick.push_back(i);
		if((int)pick.size()>K)continue;
		if(!noCycle(pick))continue;
		int w=0;
		for(int id:pick)w+=ew[id];
		ans=max(ans,w);
	}
	cout<<ans<<'\n';
	return 0;
}
