//
// 题目：P1536 村村通
// https://www.luogu.com.cn/problem/P1536
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};          // n<1000 (+5 松弛)

int f[mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int n,m;cin>>n&&n;)        // 读到单独的 0 结束
	{
		cin>>m;
		for(int i=0;i<n;++i)
			f[i]=i;                // 每组重置并查集(0-based)
		int cnt=n;                 // 连通块数,初始 n 个孤点
		for(int j=0,u,v;j<m;++j)
		{
			cin>>u>>v;--u;--v;
			int ru=getrt(u),rv=getrt(v);
			if(ru!=rv)
			{
				f[ru]=rv;
				--cnt;             // 每合并一次,块数减一
			}
		}
		cout<<cnt-1<<'\n';         // 把 cnt 个连通块串起来需 cnt-1 条边
	}
	return 0;
}
