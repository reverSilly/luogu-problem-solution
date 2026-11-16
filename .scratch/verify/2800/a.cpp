// 题目：P2800 又上锁妖塔
// 规则翻译：动作序列中任意两个跳不相邻；爬第 i 层付 h[i]，跳免费
// f[i][0]：到 i 层末步爬（力气满，可跳）；f[i][1]：末步跳（力气空，只能爬）
// f[i][0]=min(f[i-1][0],f[i-1][1])+h[i]   爬从两状态来
// f[i][1]=min(f[i-1][0],f[i-2][0])        跳只从状态 0 来，跳 1 层 / 跳 2 层
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
	f[0][0]=0;	// 地面：力气满，首步可跳
	f[0][1]=inf;	// 不可能「跳着到达地面」，必须显式填（全局默认 0 是陷阱）
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
