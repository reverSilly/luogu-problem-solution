// 题目：P3083 [USACO13OPEN] Luxury River Cruise S
// 一整遍方向序列 = 函数 f；重复 K 遍 = f^K，倍增预处理后按 K 的二进制位跳
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxk{35},mxm{505};

int n,m,K,to[mxn][2],f[mxk][mxn];	// f[j][u] = 从 u 走 2^j 遍序列后的港口
bool dir[mxm];	// 0=L, 1=R

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>K;
	for(int i=0;i<n;++i)
	{
		cin>>to[i][0]>>to[i][1];
		--to[i][0];--to[i][1];
	}
	for(int i=0;i<m;++i)
	{
		char c;cin>>c;
		dir[i]=(c=='R');
	}
	for(int u=0;u<n;++u)
	{
		int cur=u;
		for(int i=0;i<m;++i)
			cur=to[cur][dir[i]];
		f[0][u]=cur;
	}
	for(int j=1;j<30;++j)	// K<=1e9<2^30，第 0..29 层足够
		for(int u=0;u<n;++u)
			f[j][u]=f[j-1][f[j-1][u]];
	int cur=0;	// 从 1 号港口出发（0-based）
	for(int j=0;j<30;++j)
		if(K>>j&1)
			cur=f[j][cur];
	cout<<cur+1<<'\n';
	return 0;
}
