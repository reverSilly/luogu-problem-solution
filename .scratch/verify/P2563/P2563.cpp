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

	for(int i=2;i<mxn;++i)	// 线性筛
	{
		if(!comp[i])pri[cnt++]=i;
		for(int j=0;j<cnt&&i*pri[j]<mxn;++j)
		{
			comp[i*pri[j]]=true;
			if(i%pri[j]==0)break;
		}
	}
	dp[0]=1;
	for(int k=0;k<cnt;++k)	// 外层枚举质数 ⇒ 无序（本质不同）
		for(int j=pri[k];j<mxn;++j)
			dp[j]+=dp[j-pri[k]];
	while(cin>>n)
		cout<<dp[n]<<'\n';
	return 0;
}