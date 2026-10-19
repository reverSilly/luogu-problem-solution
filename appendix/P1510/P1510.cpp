//
// 题目：P1510 精卫填海
// https://www.luogu.com.cn/problem/P1510
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};   // n<=1e4 (+5 松弛)
constexpr int mxc{10005};   // c<=1e4 (+5 松弛)

int v,n,c;
int k[mxn],mm[mxn];          // k=木石体积, mm=把它衔到东海消耗的体力
int dp[mxc];                 // dp[j]=总体力不超过 j 时能填的最大体积

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>v>>n>>c;
	for(int i=0;i<n;++i)
		cin>>k[i]>>mm[i];

	for(int i=0;i<n;++i)
		for(int j=c;j>=mm[i];--j)
			dp[j]=max(dp[j],dp[j-mm[i]]+k[i]);

	for(int j=0;j<=c;++j)
		if(dp[j]>=v)
		{
			cout<<c-j<<'\n';   // 最先满足的体积对应最小体力消耗
			return 0;
		}
	cout<<"Impossible\n";
	return 0;
}
