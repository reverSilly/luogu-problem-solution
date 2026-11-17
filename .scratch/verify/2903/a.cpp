// 题目：P2903 [USACO08MAR] The Loathesome Hay Baler S
// 相切判定 dist²==(ri+rj)²；每个齿轮恰被一个齿轮带动 ⇒ 相切图是树，BFS 父指针回溯唯一传动路径
// |S_i|=10000*r0/r_i（传动比叠乘望远镜）；long double 求和后四舍五入（数据保证和为整数）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1055};

int n,Xt,Yt,x[mxn],y[mxn],r[mxn],prv[mxn];
vector<int>graph[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>Xt>>Yt;
	int rt=-1,gt=-1;
	for(int i=0;i<n;++i)
	{
		cin>>x[i]>>y[i]>>r[i];
		if(x[i]==0&&y[i]==0)
			rt=i;
		if(x[i]==Xt&&y[i]==Yt)
			gt=i;
	}
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			i64 dx=x[i]-x[j],dy=y[i]-y[j],rs=r[i]+r[j];
			if(dx*dx+dy*dy==rs*rs)	// 外切才传动，整数判等无精度问题
			{
				graph[i].push_back(j);
				graph[j].push_back(i);
			}
		}
	fill_n(prv,n,1145141919);
	queue<int>Q;
	Q.push(rt);vis[rt]=true;prv[rt]=rt;	// 根的父指针指向自己，回溯终止标志
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(!vis[v])
			{
				vis[v]=true;prv[v]=u;
				Q.push(v);
			}
	}
	long double ans=0;
	for(int u=gt;;u=prv[u])
	{
		ans+=10000.0L*r[rt]/r[u];	// 望远镜：|S_u|=10000*r[rt]/r[u]，与路径中间半径无关
		if(u==rt)
			break;
	}
	cout<<(i64)llroundl(ans)<<'\n';
	return 0;
}
