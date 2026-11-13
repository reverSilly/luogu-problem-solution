#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
const int w[6]={1,2,3,5,10,20};

int a[6];
bool dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int S=0;
	for(int i=0;i<6;++i)
	{
		cin>>a[i];
		S+=a[i]*w[i];
	}
	dp[0]=true;
	for(int i=0;i<6;++i)
		for(int k=0;k<a[i];++k)
			for(int s=S;s>=w[i];--s)
				dp[s]|=dp[s-w[i]];
	int ans=0;
	for(int s=1;s<=S;++s)
		if(dp[s])
			++ans;
	cout<<"Total="<<ans<<'\n';
	return 0;
}
