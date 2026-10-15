#include<bits/stdc++.h>
using namespace std;

struct P{int x,y;}pts[15];
int n,path[15],ans;
bool vis[15];

int cross(P a,P b,P c)
{
	return(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
bool inter(P a,P b,P c,P d)
{
	int cp1=cross(a,b,c);
	int cp2=cross(a,b,d);
	int cp3=cross(c,d,a);
	int cp4=cross(c,d,b);
	return(cp1>0&&cp2<0||cp1<0&&cp2>0)&&(cp3>0&&cp4<0||cp3<0&&cp4>0);
}
bool ok(int cur,int pos)
{
	for(int i=0;i<pos-1;++i)
		if(inter(pts[cur],pts[path[pos]],
			pts[path[i]],pts[path[i+1]]))
			return 0;
	return 1;
}
void dfs(int cur,int pos)
{
	if(pos==n-1)
	{
		for(int i=1;i<pos-1;++i)
			if(inter(pts[path[pos]],pts[path[0]],
				pts[path[i]],pts[path[i+1]]))
				return;
		++ans;
		return;
	}
	for(int i=0;i<n;++i)
	{
		if(vis[i])continue;
		path[pos+1]=i;
		if(!ok(i,pos))continue;
		vis[i]=1;
		dfs(i,pos+1);
		vis[i]=0;
	}
}
int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	int x,y;
	while(cin>>x>>y)
	{
		pts[n]={x,y};
		++n;
	}
	int st=n-1;
	vis[st]=1;
	path[0]=st;
	dfs(st,0);
	cout<<ans/2<<'\n';
	return 0;
}
