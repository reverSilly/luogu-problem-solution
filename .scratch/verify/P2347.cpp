// 题目：P2347 [NOIP 1996 提高组] 砝码称重
// 思路：可表示重量只有 0..1000 共 1001 个「状态」，把「枚举组合」换成「按重量去重」。
//       把同一重量的若干枚砝码逐枚看作 0/1 物品，做可行性背包；下标倒序保证每枚只用一次。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int w[6]{1,2,3,5,10,20};

int a[6];
bool dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int S=0;
	for(int i=0;i<6;++i)
	{
		cin>>a[i];
		S+=a[i]*w[i];
	}
	dp[0]=true;
	for(int i=0;i<6;++i)
		for(int k=0;k<a[i];++k)
			for(int s=S;s>=w[i];--s)
				dp[s]|=dp[s-w[i]];
	int ans=0;
	for(int s=1;s<=S;++s)
		if(dp[s])
			++ans;
	cout<<"Total="<<ans<<'\n';
	return 0;
}
