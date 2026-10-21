//
// 题目：P1612 [yLOI2018] 树上的链
// https://www.luogu.com.cn/problem/P1612
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005},lg{18};    // n<=1e5, 2^17>1e5

int n;
int p[mxn],w[mxn],c[mxn],dep[mxn],up[lg][mxn];
i64 s[mxn];                          // 根到该点的点权和

i64 above(int x){return x?s[p[x]]:0;}  // x 的父节点的前缀和(根记 0)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	p[0]=0;
	for(int i=1;i<n;++i)
	{
		cin>>p[i];
		--p[i];
	}
	for(int i=0;i<n;++i)
		cin>>w[i];
	for(int i=0;i<n;++i)
		cin>>c[i];

	dep[0]=0;
	s[0]=w[0];
	up[0][0]=0;
	for(int i=1;i<n;++i)             // p[i]<i, 直接按编号递推
	{
		dep[i]=dep[p[i]]+1;
		s[i]=s[p[i]]+w[i];
		up[0][i]=p[i];
	}
	for(int k=1;k<lg;++k)
		for(int i=0;i<n;++i)
			up[k][i]=up[k-1][up[k-1][i]];

	for(int u=0;u<n;++u)
	{
		i64 thr=s[u]-c[u];           // 需要 父(a) 的前缀和 >= thr
		int a=u;
		for(int k=lg-1;k>=0;--k)     // 越往上 above 越小, 二分式上跳
			if(above(up[k][a])>=thr)
				a=up[k][a];
		cout<<dep[u]-dep[a]+1<<(u+1==n?'\n':' ');
	}
	return 0;
}
