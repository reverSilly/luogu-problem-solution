//
// 题目：P1866 编号
// https://www.luogu.com.cn/problem/P1866
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{55};
constexpr int mod{1000000007};

int n;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	i64 ans=1;
	for(int i=0;i<n;++i)
	{
		int ways=a[i]-i;
		if(ways<=0)
		{
			ans=0;
			break;
		}
		ans=ans*ways%mod;
	}
	cout<<ans<<'\n';
	return 0;
}
