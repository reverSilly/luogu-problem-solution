//
// 题目：P1556 [USACO12MAR] 幸福的路 Connect the Cows B
// https://www.luogu.com.cn/problem/P1556
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};            // n <= 10 (+5 松弛)

int n;
int x[mxn],y[mxn];
i64 ans;

int sgn(int a){return (a>0)-(a<0);}   // 方向符号

void dfs(int u,int inx,int iny,int mask)
{
	if(mask==(1<<n)-1)
	{
		if(x[u]!=0&&y[u]!=0)      // 末点须与原点同行/同列才能返回
			return;
		int ox=sgn(0-x[u]),oy=sgn(0-y[u]);
		if(ox!=inx||oy!=iny)      // 在最后一头牛处方向必须改变(90 或 180)
			++ans;
		return;
	}
	for(int v=0;v<n;++v)
	{
		if(mask>>v&1)
			continue;
		if(x[u]!=x[v]&&y[u]!=y[v])
			continue;             // 必须是同一行或同一列
		int ox=sgn(x[v]-x[u]),oy=sgn(y[v]-y[u]);
		if(ox==inx&&oy==iny)      // 直行(方向不变)=> 当前牛没有转弯, 非法
			continue;
		dfs(v,ox,oy,mask|1<<v);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	ans=0;
	for(int v=0;v<n;++v)
	{
		if(x[v]!=0&&y[v]!=0)
			continue;             // 第一头牛须与原点同行/同列
		int ox=sgn(x[v]-0),oy=sgn(y[v]-0);
		dfs(v,ox,oy,1<<v);
	}
	cout<<ans<<'\n';
	return 0;
}
