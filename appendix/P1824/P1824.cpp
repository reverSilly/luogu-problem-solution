//
// 题目：P1824 [USACO05FEB] 进击的奶牛 Aggressive Cows G
// https://www.luogu.com.cn/problem/P1824
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m;
int a[mxn];

bool ok(int d)
{
	int cnt=1,last=a[0];
	for(int i=1;i<n;++i)
		if(a[i]-last>=d)
		{
			++cnt;
			last=a[i];
			if(cnt>=m)
				return true;
		}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	int l=0,r=a[n-1]-a[0],ans=0;
	while(l<=r)
	{
		int mid=l+(r-l)/2;
		if(ok(mid))
		{
			ans=mid;
			l=mid+1;
		}
		else
			r=mid-1;
	}
	cout<<ans<<'\n';
	return 0;
}
