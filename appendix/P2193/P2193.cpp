// 题目：P2193 HXY和序列
// 思路：f[j][i] = 长度为 j 且末项为 i 的方案数
//       f[1][i] = 1，f[j][i] = sum_{d|i} f[j-1][d]
//       用「枚举 d 的倍数」代替「枚举 i 的因子」，把每层压到 O(n log n)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};
constexpr int p{1000000007};

int n,len;
int f[mxn],g[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>len;
	for(int i=1;i<=n;++i)f[i]=1;
	for(int j=2;j<=len;++j)
	{
		memset(g,0,sizeof(g));
		for(int d=1;d<=n;++d)
		{
			if(!f[d])continue;
			for(int m=d;m<=n;m+=d)
			{
				g[m]+=f[d];
				if(g[m]>=p)g[m]-=p;
			}
		}
		for(int i=1;i<=n;++i)f[i]=g[i];
	}
	int ans=0;
	for(int i=1;i<=n;++i)
	{
		ans+=f[i];
		if(ans>=p)ans-=p;
	}
	cout<<ans<<'\n';
	return 0;
}
