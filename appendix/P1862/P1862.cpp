//
// 题目：P1862 输油管道问题
// https://www.luogu.com.cn/problem/P1862
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n;
int y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		int x;
		cin>>x>>y[i];
	}
	sort(y,y+n);
	int mid=y[n/2];
	i64 ans=0;
	for(int i=0;i<n;++i)
		ans+=abs(y[i]-mid);
	cout<<ans<<'\n';
	return 0;
}
