// 题目：P2563 [AHOI2001] 质数和分解
// 思路：本质不同 = 顺序无关 = 把 n 分拆成质数多重集 ⇒ 完全背包计数。
//       外层枚举质数、内层正序 dp[j]+=dp[j-p]，每种多重集按质数升序唯一构造 ⇒ 无序。
//       质数<=n 冗余（和为 n、加数>=2）⇒ 一次预处理 dp[0..200]，O(1) 回答每个询问。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};

int n,pri[mxn],cnt;
bool comp[mxn];
i64 dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=2;i<mxn;++i)	// 线性筛：每个合数被最小质因子筛一次
	{
		if(!comp[i])pri[cnt++]=i;
		for(int j=0;j<cnt&&i*pri[j]<mxn;++j)
		{
			comp[i*pri[j]]=true;
			if(i%pri[j]==0)break;
		}
	}
	dp[0]=1;	// 空多重集是种子
	for(int k=0;k<cnt;++k)	// 外层质数 ⇒ 无序；内层正序 ⇒ 可重复用
		for(int j=pri[k];j<mxn;++j)
			dp[j]+=dp[j-pri[k]];
	while(cin>>n)
		cout<<dp[n]<<'\n';
	return 0;
}
