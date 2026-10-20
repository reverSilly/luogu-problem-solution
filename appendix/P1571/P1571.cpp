//
// 题目：P1571 眼红的 Medusa
// https://www.luogu.com.cn/problem/P1571
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};    // n,m<=1e5 (+5 松弛)

int n,m;
int a[mxn],b[mxn];            // a 保持获奖次序;b 排序后用于二分

bool has(int x)               // b 中是否存在 x
{
	int l=0,r=m;
	while(l<r)
	{
		int mid=(l+r)>>1;
		if(b[mid]<x)
			l=mid+1;
		else
			r=mid;
	}
	return l<m&&b[l]==x;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
		cin>>b[i];
	sort(b,b+m);

	bool first=true;
	for(int i=0;i<n;++i)
		if(has(a[i]))         // 只依赖"是否在 b 中",与 b 内部次序无关
		{
			if(!first)
				cout<<' ';
			cout<<a[i];
			first=false;
		}
	cout<<'\n';
	return 0;
}
