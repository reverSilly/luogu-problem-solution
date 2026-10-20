// 题目：P1582 倒水 —— 暴力基准(真·模拟,不借用 popcount)
// 状态=各水位(2 的幂)的瓶数分布;从 n+x 个 1 升瓶出发,
// DFS 枚举所有"两瓶等量合并"的顺序,取最少剩余瓶数。
// 仅适用于小规模(n+x<=1023),超出范围直接报错退出。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int LV{10};    // 水位层数 2^0..2^9,故 n+x<=1023

i64 n;
int k;

map<array<int,LV>,int>memo;   // 瓶数分布 -> 从该分布出发的最少剩余瓶数

int dfs(array<int,LV>c)
{
	auto it=memo.find(c);
	if(it!=memo.end())
		return it->second;
	int ans=0;
	for(int j=0;j<LV;++j)
		ans+=c[j];            // 就地停手也是一种终局
	for(int j=0;j+1<LV;++j)
		if(c[j]>=2)           // 两瓶 2^j 合成一瓶 2^(j+1),瓶数 -1
		{
			auto d=c;
			d[j]-=2;
			++d[j+1];
			ans=min(ans,dfs(d));
		}
	memo[c]=ans;
	return ans;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(i64 x=0;;++x)         // 枚举买入瓶数:第一个达标的 x 即答案
	{
		if(n+x>(1<<LV)-1)
		{
			cerr<<"brute: n+x 超出小规模范围\n";
			return 1;
		}
		array<int,LV>c{};
		c[0]=(int)(n+x);      // 初始:n+x 个 1 升瓶
		if(dfs(c)<=k)
		{
			cout<<x<<'\n';
			return 0;
		}
	}
}
