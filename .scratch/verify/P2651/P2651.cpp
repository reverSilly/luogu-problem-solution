#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n;
i64 a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i];
		i64 rem=a[1];			// a2 永远在分母，判断它能否被整除
		for(int i=0;i<n&&rem>1;++i)
		{
			if(i==1)
				continue;
			rem/=gcd(rem,a[i]);	// 逐个用分子约掉 rem 的因子
		}
		cout<<(rem==1?"Yes\n":"No\n");
	}
	return 0;
}