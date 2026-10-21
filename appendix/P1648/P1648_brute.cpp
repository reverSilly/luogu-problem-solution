//
// 题目：P1648 看守
// 暴力基准：O(n^2 d) 枚举所有点对直接求曼哈顿距离最大值
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int n,d;
i64 pt[mxn][4];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>d;
	for(int i=0;i<n;++i)
		for(int j=0;j<d;++j)
			cin>>pt[i][j];

	i64 ans=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			i64 cur=0;
			for(int k=0;k<d;++k)
				cur+=llabs(pt[i][k]-pt[j][k]);
			ans=max(ans,cur);
		}
	cout<<ans<<'\n';
	return 0;
}
