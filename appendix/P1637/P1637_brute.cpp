//
// 题目：P1637 三元上升子序列
// 暴力基准：O(n^2) 枚举中间位置 j 并向两侧线性扫描计数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];

	i64 ans=0;
	for(int j=0;j<n;++j)
	{
		i64 l=0,r=0;
		for(int i=0;i<j;++i)
			if(a[i]<a[j])
				++l;
		for(int k=j+1;k<n;++k)
			if(a[j]<a[k])
				++r;
		ans+=l*r;
	}
	cout<<ans<<'\n';
	return 0;
}
