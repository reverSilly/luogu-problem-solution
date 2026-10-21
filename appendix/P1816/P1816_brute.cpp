//
// 题目：P1816 忠诚
// https://www.luogu.com.cn/problem/P1816
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int m,n;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=1;i<=m;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
	{
		int l,r;
		cin>>l>>r;
		if(l>r)
			swap(l,r);
		int ans{a[l]};
		for(int j=l;j<=r;++j)
			ans=min(ans,a[j]);
		if(i)
			cout<<' ';
		cout<<ans;
	}
	cout<<'\n';
	return 0;
}
