// 题目：P2683 [AHOI2015 初中组] 小岛
// 思路：只加边不删边 ⇒ 动态 Floyd：加入边 (u,v,e) 后，
//       d[i][j]=min(d[i][j], d[i][u]+e+d[v][j], d[i][v]+e+d[u][j])，O(N^2) 一次。
//       新最短路要么不走新边，要么恰好走一次（边权正，走两次可抄近路）。
//       原地更新安全：存入的每个值都是新图里某条途径的长度（不会偏小），
//       且旧值只减不增（不会偏大），两边夹逼即得正确。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105},inf{1145141919};

int n,m,d[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			d[i][j]=(i==j?0:inf);	// 不可达必须显式填 inf，全局默认 0 会被当成零距离
	for(int q=0,op,u,v,e;q<m;++q)
	{
		cin>>op>>u>>v;--u;--v;
		if(op)
		{
			cin>>e;
			if(e>=d[u][v])
				continue;	// 已有更短的 u-v 通路，这条重边毫无用处
			d[u][v]=d[v][u]=e;
			for(int i=0;i<n;++i)
				for(int j=0;j<n;++j)
				{
					// inf+e+inf 约 2.3e9 会爆 int，中间量必须提到 i64
					i64 t=min((i64)d[i][u]+e+d[v][j],(i64)d[i][v]+e+d[u][j]);
					if(t<d[i][j])
						d[i][j]=(int)t;
				}
		}
		else
			cout<<(d[u][v]>=inf?-1:d[u][v])<<'\n';
	}
	return 0;
}