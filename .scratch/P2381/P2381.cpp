#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,d[mxn];
i64 P[mxn*2];		// P[k]=前 k 段顺时针距离之和，k∈[0,2n]

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 S=0;
	for(int i=0;i<n;++i)
	{
		cin>>d[i];
		S+=d[i];
	}
	P[0]=0;
	for(int i=0;i<2*n;++i)
		P[i+1]=P[i]+d[i%n];
	// 对每个起点 i，找弧长最接近 S/2 的 j（j 单调不减）
	i64 ans=0;
	int j=0;
	for(int i=0;i<n;++i)
	{
		if(j<i)
			j=i;
		while(j+1<=i+n-1&&2*(P[j+1]-P[i])<=S)
			++j;
		if(j>=i+1)							// 不超过半圈的最后一只
			ans=max(ans,P[j]-P[i]);
		if(j+1<=i+n-1)						// 超过半圈的第一只
			ans=max(ans,S-(P[j+1]-P[i]));
	}
	cout<<ans<<'\n';
	return 0;
}
