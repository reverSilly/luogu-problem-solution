//
// 题目：P1833 樱花
// https://www.luogu.com.cn/problem/P1833
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxt{1005};

int T;
int dp[mxt];

void addfull(int tw,int tv)
{
	for(int j=tw;j<=T;++j)
		dp[j]=max(dp[j],dp[j-tw]+tv);
}

void add01(int tw,int tv)
{
	if(tw>T)
		return;
	for(int j=T;j>=tw;--j)
		dp[j]=max(dp[j],dp[j-tw]+tv);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int h1,m1,h2,m2,n;
	char c1,c2;
	cin>>h1>>c1>>m1>>h2>>c2>>m2>>n;
	T=(h2*60+m2)-(h1*60+m1);
	if(T<0)
		T=0;
	for(int i=0;i<n;++i)
	{
		int t,c,p;
		cin>>t>>c>>p;
		if(t<=0||t>T||c<0)
			continue;
		if(p==0||t*1LL*p>=T)
			addfull(t,c);
		else
		{
			int left=p,k=1;
			while(left>0)
			{
				int take=min(k,left);
				add01(take*t,take*c);
				left-=take;
				k<<=1;
			}
		}
	}
	int ans=0;
	for(int j=0;j<=T;++j)
		ans=max(ans,dp[j]);
	cout<<ans<<'\n';
	return 0;
}
