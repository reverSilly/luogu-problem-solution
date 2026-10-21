//
// 题目：P1650 田忌赛马
// https://www.luogu.com.cn/problem/P1650
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n,a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>b[i];

	sort(a,a+n);
	sort(b,b+n);

	int la=0,ra=n-1,lb=0,rb=n-1,ans=0;
	for(int i=0;i<n;++i)
	{
		if(a[ra]>b[rb])
		{
			ans+=200;
			--ra;--rb;
		}
		else if(a[ra]<b[rb])
		{
			ans-=200;
			++la;--rb;
		}
		else
		{
			if(a[la]>b[lb])
			{
				ans+=200;
				++la;++lb;
			}
			else
			{
				if(a[la]<b[rb])
					ans-=200;
				++la;--rb;
			}
		}
	}
	cout<<ans<<'\n';
	return 0;
}
