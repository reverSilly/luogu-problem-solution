// 独立解法：Euler 公式  区域数 R = E - V + C + 1，答案 = R-1 = E - V + C
// V=不同格点数, E=不同边数, C=围栏图连通块数(DSU)
#include<bits/stdc++.h>
using namespace std;

int n;string s;
map<pair<int,int>,int>id;
vector<int>f;
int getrt(int x){return f[x]==x?x:f[x]=getrt(f[x]);}

int main()
{
	cin.tie(nullptr);cout.tie(nullptr);ios::sync_with_stdio(false);
	cin>>n>>s;
	int x=0,y=0;
	set<pair<int,int>>pts;
	set<pair<pair<int,int>,pair<int,int>>>edges;
	pts.insert({0,0});
	for(int i=0;i<n;++i)
	{
		int nx=x,ny=y;char c=s[i];
		if(c=='N')++ny;else if(c=='S')--ny;else if(c=='E')++nx;else --nx;
		auto a=make_pair(x,y),b=make_pair(nx,ny);
		if(a>b)swap(a,b);
		edges.insert({a,b});pts.insert(b);
		x=nx;y=ny;
	}
	int V=pts.size(),E=edges.size();
	for(auto&p:pts){int k=f.size();id[p]=k;f.push_back(k);}
	for(auto&e:edges){int u=getrt(id[e.first]),v=getrt(id[e.second]);if(u!=v)f[u]=v;}
	set<int>roots;for(auto&p:pts)roots.insert(getrt(id[p]));
	int C=roots.size();
	cout<<E-V+C<<'\n';
	return 0;
}