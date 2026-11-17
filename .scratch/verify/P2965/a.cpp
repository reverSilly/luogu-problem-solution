// 题目：P2965 [USACO09NOV] The Grand Farm-off S
// 按 (U 降序, W 升序) 排序取前 N：先最大化有用度之和，再最小化体重和
// i^5 最大约 7.6e30，必须逐步取模；每步乘积 < 1e18，i64 恰好装得下
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1500005};

int n;
i64 a,b,c,d,e,f,g,h,M;
pair<int,int>cow[mxn];	// {U,W}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>a>>b>>c>>d>>e>>f>>g>>h>>M;
	for(int i=0;i<3*n;++i)
	{
		i64 w=0,u=0;
		if(d)	// 题面 1<=W<=d 隐含 d>=1；d=0 时按 0 处理以防除零
		{
			i64 t=i%d,t2=t*t%d,t5=t2*t%d*t2%d;
			w=(a*t5+b*t2+c)%d;
		}
		if(h)
		{
			i64 t=i%h,t2=t*t%h,t3=t2*t%h,t5=t3*t2%h;
			u=(e*t5+f*t3+g)%h;
		}
		cow[i]={u,w};
	}
	sort(cow,cow+3*n,[](auto x,auto y){return x.first!=y.first?x.first>y.first:x.second<y.second;});
	i64 ans=0;
	for(int i=0;i<n;++i)
		ans+=cow[i].second;	// 和 < 5e5*1e9 = 5e14，i64 安全
	cout<<ans%M<<'\n';
	return 0;
}