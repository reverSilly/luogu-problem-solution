// 题目：P2618 数字工程
// 思路：两种操作都严格减小 n ⇒ 最优子结构：f[i]=1+min(f[i-1], min_{p|i} f[i/p])。
//       线性筛预处理最小质因子 spf，正推 f[2..1e6]；对每个 i 沿 spf 链枚举互异质因子。
//       多组数据（EOF 驱动）：预处理一次，每问 O(1)。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int f[mxn],spf[mxn],pr[mxn],cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	// 线性筛：spf[x] = x 的最小质因子（质数处 spf[p]=p）
	for(int i=2;i<mxn;++i)
	{
		if(!spf[i])
			spf[i]=i,pr[cnt++]=i;
		for(int j=0;j<cnt&&(i64)i*pr[j]<mxn;++j)
		{
			spf[i*pr[j]]=pr[j];
			if(i%pr[j]==0)
				break;			// 保证每个合数只被最小质因子筛一次
		}
	}
	f[1]=0;
	for(int i=2;i<mxn;++i)
	{
		f[i]=f[i-1]+1;			// 操作 1：减 1
		for(int x=i;x>1;)
		{
			int p=spf[x];
			f[i]=min(f[i],f[i/p]+1);	// 操作 2：除以质因子 p，i/p<i 已算好
			for(;x%p==0;x/=p);		// 剥掉 p 的幂，只枚举互异质因子
		}
	}
	int n;
	while(cin>>n)
		cout<<f[n]<<'\n';
	return 0;
}
