//
// 题目：P1599 [USACO09MAR] 结算日 Payback B
// https://www.luogu.com.cn/problem/P1599
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};       // N<=100000 (+5 松弛)

int n,d[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int ans=n;                   // 一路向右走到最后一头奶牛: 基础距离 N
	i64 pre=0;                   // 前缀收支和
	for(int i=1;i<=n;++i)
	{
		cin>>d[i];
		pre+=d[i];
		if(pre<0)                // 前缀和 <0 的边 (i,i+1) 必须被向左穿越一次
			ans+=2;
	}
	cout<<ans<<'\n';
	return 0;
}
