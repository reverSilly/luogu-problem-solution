// 题目：P2192 HXY玩卡片
// 思路：90 = 9 * 10；末位必须是 0，数字和必须是 9 的倍数
//       数字和 = 5 * (5 的个数) => 5 的个数必须是 9 的倍数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,c5,c0;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		int x;cin>>x;
		if(x==5)++c5;
		else ++c0;
	}
	// 没有 0 就凑不出末位 0，连整数 0 都排不出来
	if(c0==0)
	{
		cout<<-1<<'\n';
		return 0;
	}
	// 5 的个数必须是 9 的倍数，取最大的那个（位数越多数越大）
	int k=c5/9*9;
	if(k==0)
	{
		// 凑不出含 5 的合法数，只有整数 0 合法
		cout<<0<<'\n';
		return 0;
	}
	// 同样的位数下 5 全部前置最大；0 全部用上（多一位就更大）
	for(int i=0;i<k;++i)cout<<'5';
	for(int i=0;i<c0;++i)cout<<'0';
	cout<<'\n';
	return 0;
}
