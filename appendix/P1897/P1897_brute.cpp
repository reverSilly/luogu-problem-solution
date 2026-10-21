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
	i64 ans=0;
	int cur=0;
	for(int i=0;i<n;){
		int j=i;
		while(j<n&&a[j]==a[i]){
			++j;
		}
		int f=a[i];
		int cnt=j-i;
		ans+=(i64)(f-cur)*6;
		ans+=5;
		ans+=cnt;
		cur=f;
		i=j;
	}
	ans+=(i64)cur*4;
	cout<<ans<<'\n';
	return 0;
}
