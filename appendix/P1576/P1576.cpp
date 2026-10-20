//
// 题目：P1576 最小花费
// https://www.luogu.com.cn/problem/P1576
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};        // n<=2000 (+5 松弛)
constexpr int mxm{100005};      // m<=100000 (+5 松弛)
constexpr double inf{1e18};

int n,m,cnt;
struct{int v,nx;double w;}E[mxm*2];
int H[mxn];
double dis[mxn];
bool vis[mxn];

void addedge(int u,int v,double w)
{
	E[cnt]={v,H[u],w};H[u]=cnt++;
	E[cnt]={u,H[v],w};H[v]=cnt++;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	fill_n(&H[0],mxn,-1);        // 链式前向星表头默认应为 -1,全局 int 默认 0 必须显式填充
	cin>>n>>m;
	for(int i=0,x,y,z;i<m;++i)
	{
		cin>>x>>y>>z;--x;--y;
		addedge(x,y,-log(1.0-z/100.0));   // 手续费 z% => 边长 -ln(1-z/100) > 0
	}
	int A,B;
	cin>>A>>B;--A;--B;
	fill_n(&dis[0],mxn,inf);
	dis[A]=0.0;
	priority_queue<pair<double,int>,vector<pair<double,int>>,greater<>>Q;
	Q.push({0.0,A});
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		if(u==B)
			break;
		for(int i=H[u];i!=-1;i=E[i].nx)
		{
			int v=E[i].v;
			double w=E[i].w;
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				Q.push({dis[v],v});
			}
		}
	}
	// 边长取 -ln(因子),最短路即 ln 最小 = 乘积最大方案;B 到账 100 => 最少付出 100/(最大乘积)
	cout<<fixed<<setprecision(8)<<100.0*exp(dis[B])<<'\n';
	return 0;
}
