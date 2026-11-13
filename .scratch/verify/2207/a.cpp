#include<bits/stdc++.h>
using namespace std;

constexpr int mxk{1005};
constexpr int inf{1145141919};

int n,k;
int a[mxk],b[mxk];   // 第i对：a[i]<=b[i]

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<k;++i)
	{
		cin>>a[i]>>b[i];
		if(a[i]>b[i])
			swap(a[i],b[i]);
	}
	int cur=1,ans=0;
	while(cur<=n)
	{
		int lim=inf;   // lim=min{b_i : a_i>=cur}
		for(int i=0;i<k;++i)
			if(a[i]>=cur)
				lim=min(lim,b[i]);
		++ans;
		if(lim==inf)
			break;
		cur=lim;
	}
	cout<<ans<<'\n';
	return 0;
}
