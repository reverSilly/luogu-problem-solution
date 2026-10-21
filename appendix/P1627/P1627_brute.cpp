//
// 题目：P1627 [CQOI2009] 中位数 （暴力对拍用）
// 枚举所有奇数长度子区间，排序后取中间元素
#include<bits/stdc++.h>
using namespace std;

int n,b,a[105],t[105];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>b;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int ans=0;
	for(int l=0;l<n;++l)
		for(int r=l;r<n;r+=2)
		{
			int len=r-l+1;
			for(int k=l;k<=r;++k)
				t[k-l]=a[k];
			sort(t,t+len);
			if(t[len/2]==b)
				++ans;
		}
	cout<<ans<<'\n';
	return 0;
}
