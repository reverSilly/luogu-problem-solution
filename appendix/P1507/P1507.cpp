//
// 题目：P1507 NASA的食物计划
// https://www.luogu.com.cn/problem/P1507
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};            // 食品数 n<=50 (+5 松弛)
constexpr int mxv{405};           // 体积/质量上限 <=400 (+5 松弛)

int H,T,n;
int h[mxn],t[mxn],k[mxn];
int dp[mxv][mxv];                 // dp[a][b]=体积至多 a、质量至多 b 的最大卡路里,初值 0(=一件不选)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>H>>T>>n;
	for(int i=0;i<n;++i)
		cin>>h[i]>>t[i]>>k[i];

	for(int i=0;i<n;++i)
	{
		if(h[i]>H||t[i]>T)
			continue;             // 单独一件都塞不下,永远用不上
		for(int a=H;a>=h[i];--a)  // 两维都倒序:保证读到的 dp 是本件之前的旧值(每件至多用一次)
			for(int b=T;b>=t[i];--b)
				dp[a][b]=max(dp[a][b],dp[a-h[i]][b-t[i]]+k[i]);
	}

	cout<<dp[H][T]<<'\n';         // 至多语义,无需再对 h<=H,t<=T 取 max
	return 0;
}
