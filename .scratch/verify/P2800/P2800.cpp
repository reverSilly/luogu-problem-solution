// 题目：P2800 又上锁妖塔
// f[i][0]：到第 i 层且最后一步是爬（有力气，可跳）；f[i][1]：最后一步是跳（必须爬）
// f[i][0]=min(f[i-1][0],f[i-1][1])+h[i]
// f[i][1]=min(f[i-1][0],f[i-2][0])   跳 1 层 / 跳 2 层，起跳前必须是状态 0
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005},inf{1145141919};

int n,h[mxn],f[mxn][2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
		cin>>h[i];
	f[0][0]=0;	// 地面：有力气
	f[0][1]=inf;	// 不可能「跳着到达地面」
	for(int i=1;i<=n;++i)
	{
		f[i][0]=min(f[i-1][0],f[i-1][1])+h[i];
		f[i][1]=f[i-1][0];
		if(i>=2)
			f[i][1]=min(f[i][1],f[i-2][0]);
	}
	cout<<min(f[n][0],f[n][1])<<'\n';
	return 0;
}