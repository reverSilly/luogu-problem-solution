//
// 题目：P1566 加等式
// https://www.luogu.com.cn/problem/P1566
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};            // m<=30 (+5 松弛)
constexpr int mxv{1005};          // x<=1000 (+5 松弛)

int t,m;
int x[mxn];
i64 dp[mxv];                      // 子集和计数(只算到 x[i] 即可)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>t;
	while(t--)
	{
		cin>>m;
		for(int i=0;i<m;++i)
			cin>>x[i];
		i64 ans=0;
		for(int i=0;i<m;++i)
		{
			fill_n(&dp[0],mxv,0);   // 多测必须清零(全局默认 0 仅首次有效)
			dp[0]=1;
			for(int j=0;j<m;++j)
			{
				if(j==i)
					continue;
				for(int s=x[i];s>=x[j];--s)
					dp[s]+=dp[s-x[j]];
			}
			ans+=dp[x[i]];          // 其余元素凑出 x[i] 的无序方案数
		}
		cout<<ans<<'\n';
	}
	return 0;
}
