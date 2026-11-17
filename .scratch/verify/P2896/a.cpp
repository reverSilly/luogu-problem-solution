// 题目：P2896 Eating Together S
// 最少修改使序列不降或不升；值域只有 1..3
// f[j]=前缀以值 j 结尾的最小修改数，f'[j]=(a[i]!=j)+min_{k<=j}f[k]（前缀 min 滚动）
// 不升 = 把值 v 映成 2-v（0-based）后再跑同一份不降 DP
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30005},inf{1145141919};

int n,a[mxn],f[3],g[3];

int calc(int dir)	// dir=0 不降；dir=1 值翻转后不降 = 原序列不升
{
	int v=dir?2-a[0]:a[0];
	for(int j=0;j<3;++j)
		f[j]=(v!=j);	// 第一头牛：任何结尾值都可达，无需 INF
	for(int i=1,mn;i<n;++i)
	{
		v=dir?2-a[i]:a[i];
		mn=inf;
		for(int j=0;j<3;++j)
		{
			mn=min(mn,f[j]);	// mn=min_{k<=j}f[k]
			g[j]=mn+(v!=j);
		}
		for(int j=0;j<3;++j)
			f[j]=g[j];
	}
	return min({f[0],f[1],f[2]});
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];--a[i];	// 内部 0-based：值 0,1,2
	}
	cout<<min(calc(0),calc(1))<<'\n';
	return 0;
}