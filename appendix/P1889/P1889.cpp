//
// 题目：P1889 [CEOI 1998] 士兵站队
// https://www.luogu.com.cn/problem/P1889
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n;
int x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i){
		cin>>x[i]>>y[i];
	}
	sort(x,x+n);
	sort(y,y+n);
	for(int i=0;i<n;++i){
		x[i]-=i;
	}
	sort(x,x+n);
	int mx=x[(n-1)/2];
	int my=y[(n-1)/2];
	i64 ans=0;
	for(int i=0;i<n;++i){
		ans+=abs(x[i]-mx);
		ans+=abs(y[i]-my);
	}
	cout<<ans<<'\n';
	return 0;
}
