//
// 题目：P1890 gcd 区间
// https://www.luogu.com.cn/problem/P1890
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,m;
int a[mxn];

int gg(int x,int y)
{
	while(y){
		int t=x%y;
		x=y;
		y=t;
	}
	return x;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=1;i<=n;++i){
		cin>>a[i];
	}
	while(m--){
		int l,r;
		cin>>l>>r;
		int ans=a[l];
		for(int i=l+1;i<=r;++i){
			ans=gg(ans,a[i]);
		}
		cout<<ans<<'\n';
	}
	return 0;
}
