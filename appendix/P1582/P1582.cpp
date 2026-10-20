// 题目：P1582 倒水
// https://www.luogu.com.cn/problem/P1582
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxb{37};    // 枚举位宽 0..36:N<2^31 只需 0..31,32..36 为 +5 松弛冗余

i64 n;
int k;

int pc(i64 x){return __builtin_popcountll(x);}   // 二进制 1 的个数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	i64 ans{1145141919810};   // 114514 家族哨兵:K>=1 时必被更新(答案 < 2^31)
	if(pc(n)<=k)
		ans=0;               // 一瓶不买就已达标
	for(int i=0;i<mxb;++i)
	{
		if(n>>i&1)
			continue;        // 该位已是 1,不可能是最高分歧位
		i64 t=(n>>(i+1)<<(i+1))|(1LL<<i);   // 高位保留、第 i 位补 1、低位清零
		if(pc(t)<=k)
			ans=min(ans,t-n);
	}
	cout<<ans<<'\n';
	return 0;
}
