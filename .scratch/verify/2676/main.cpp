#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005};

int n,h[mxn];
i64 B,sum;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>B;
	for(int i=0;i<n;++i)
		cin>>h[i];
	sort(h,h+n,greater<>());	// 从高到矮
	int ans=0;
	for(int i=0;i<n&&sum<B;++i)
	{
		sum+=h[i];
		++ans;
	}
	cout<<ans<<'\n';
	return 0;
}