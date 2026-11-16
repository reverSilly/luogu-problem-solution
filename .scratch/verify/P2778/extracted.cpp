// 题目：P2778 [AHOI2016初中组] 迷宫
// 思路：圆两两不交不切 ⇒ 包含关系 laminar（不相交即嵌套）。
//       一堵墙必须拆 <=> 它恰好包含两点之一：奇偶性给下界，沿包含树路径走给上界。
//       答案 = #{圆恰好包含一点}，每询问 O(N) 扫一遍 insideA^insideB 计数，无需建树。
//       坐标 1e8 ⇒ 距离平方最大 8e16，必须 i64；点不在圆上 ⇒ 严格 < 即可。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{8005};

int n,q;
i64 x[mxn],y[mxn],r2[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,r;i<n;++i)
	{
		cin>>x[i]>>y[i]>>r;
		r2[i]=(i64)r*r;		// 半径平方预处理，避免每询问重算
	}
	cin>>q;
	for(i64 a,b,c,d;q--;)
	{
		cin>>a>>b>>c>>d;
		int ans=0;
		for(int i=0;i<n;++i)
		{
			i64 da=(a-x[i])*(a-x[i])+(b-y[i])*(b-y[i]);
			i64 db=(c-x[i])*(c-x[i])+(d-y[i])*(d-y[i]);
			ans+=(da<r2[i])!=(db<r2[i]);	// 恰好包含一点 ⇒ 这堵墙横在两人之间
		}
		cout<<ans<<'\n';
	}
	return 0;
}
