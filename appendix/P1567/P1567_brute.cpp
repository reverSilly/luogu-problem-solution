#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int ans=1;
	for(int l=0;l<n;++l)          // 枚举起点,向右延伸严格上升段
	{
		int r=l;
		while(r+1<n&&a[r+1]>a[r])
			++r;
		ans=max(ans,r-l+1);
	}
	cout<<ans<<'\n';
	return 0;
}
