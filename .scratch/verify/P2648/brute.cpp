#include<bits/stdc++.h>
using namespace std;
// 独立暴力：枚举简单环判 orz；无正环时枚举简单路径取最大收益
int D,P,C,F;
vector<pair<int,int>>g[10]; // {to,weight}
bool vis[10];
int best;
bool haspos;
void cyc(int s,int u,int w,int len) // 枚举从 s 出发的简单环
{
	for(auto[v,ww]:g[u])
	{
		if(v==s&&len>=1)
		{
			if(w+ww>0)haspos=true;
			continue;
		}
		if(!vis[v])
		{
			vis[v]=true;
			cyc(s,v,w+ww,len+1);
			vis[v]=false;
		}
	}
}
void path(int u,int cur) // 枚举简单路径
{
	best=max(best,cur);
	for(auto[v,w]:g[u])
		if(!vis[v])
		{
			vis[v]=true;
			path(v,cur+w);
			vis[v]=false;
		}
}
int main()
{
	cin>>D>>P>>C>>F;
	for(int i=0,a,b;i<P;++i){cin>>a>>b;--a;--b;g[a].push_back({b,D});}
	for(int i=0,j,k,t;i<F;++i){cin>>j>>k>>t;--j;--k;g[j].push_back({k,D-t});}
	for(int s=0;s<C;++s){fill(vis,vis+C,false);vis[s]=true;cyc(s,s,0,0);}
	if(haspos){cout<<"orz\n";return 0;}
	best=0;
	for(int s=0;s<C;++s){fill(vis,vis+C,false);vis[s]=true;path(s,D);}
	cout<<best<<'\n';
	return 0;
}