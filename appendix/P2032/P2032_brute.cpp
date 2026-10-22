// 题目：P2032 扫描 —— 暴力：每次直接扫窗口求最大值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,k,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i) cin>>a[i];
	for(int i=0;i+k<=n;++i)
	{
		int mx=a[i];
		for(int j=i+1;j<i+k;++j) mx=max(mx,a[j]);
		cout<<mx<<'\n';
	}
	return 0;
}
