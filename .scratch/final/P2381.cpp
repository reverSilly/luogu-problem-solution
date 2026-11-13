// 题目：P2381 圆圆舞蹈
// 思路：把环展开成两圈做前缀和；一对奶牛的距离是 min(弧长, S-弧长)，
//       它是弧长的单峰函数（峰在半圈处）。对每个起点双指针找
//       "弧长不超过半圈的最后一个终点 j"，只需考察 j 与 j+1 两个候选。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,d[mxn];
i64 P[mxn*2];		// P[k]=前 k 段顺时针距离之和（展开成两圈），P[0]=0

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
	for(int i=0;i<2*n;++i)
		P[i+1]=P[i]+d[i%n];
	i64 ans=0;
	int j=0;
	for(int i=0;i<n;++i)
	{
		if(j<i)						// 虚拟起点：弧长 0
			j=i;
		while(j+1<=i+n-1&&2*(P[j+1]-P[i])<=S)	// 2A 与 S 比较，避免除法
			++j;
		if(j>=i+1)					// 峰左侧的最后一个
			ans=max(ans,P[j]-P[i]);
		if(j+1<=i+n-1)				// 峰右侧的第一个
			ans=max(ans,S-(P[j+1]-P[i]));
	}
	cout<<ans<<'\n';
	return 0;
}
