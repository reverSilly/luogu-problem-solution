//
// 题目：P1897 电梯里的尴尬
// https://www.luogu.com.cn/problem/P1897
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i){
		cin>>a[i];
	}
	sort(a,a+n);
	int uniq=1;
	for(int i=1;i<n;++i){
		if(a[i]!=a[i-1]){
			++uniq;
		}
	}
	i64 top=a[n-1];
	i64 ans=top*6+top*4+5LL*uniq+n;
	cout<<ans<<'\n';
	return 0;
}
