//
// 题目：P1631 序列合并
// 暴力基准：枚举全部 N^2 个和并排序取前 N 小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,a[mxn],b[mxn],all_s[mxn*mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>b[i];

	int cnt=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			all_s[cnt++]=a[i]+b[j];
	sort(all_s,all_s+cnt);
	for(int k=0;k<n;++k)
		cout<<all_s[k]<<(k+1==n?'\n':' ');
	return 0;
}
