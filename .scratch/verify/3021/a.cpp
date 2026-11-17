// 题目：P3021 [USACO11MAR] Bovine Bridge Battle S
// 180° 旋转是对合且唯一不动点是中心 ⇒ 合法四元组 = 两对「坐标和相同」的点对
// 枚举所有点对的和，排序后扫描相等段，每段长 k 贡献 C(k,2)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005},mxm{500005};	// C(1000,2)=499500

int n,x[mxn],y[mxn];
pair<int,int>s[mxm];					// 点对坐标和（=2 倍中点，避免半整数）

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	int m=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			s[m++]={x[i]+x[j],y[i]+y[j]};	// 和的范围 ±2e9，int 装得下
	sort(s,s+m);
	i64 ans=0;
	for(int i=0,k;i<m;)
	{
		k=1;
		while(i+k<m&&s[i+k]==s[i])
			++k;
		ans+=(i64)k*(k-1)/2;				// 同和的两对点自动不交，且每个四元组只有一种配对
		i+=k;
	}
	cout<<ans<<'\n';
	return 0;
}
