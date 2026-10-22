#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{35};
constexpr int lim{6005};

int n,v;
//dp[d]：在"好感之差 A-B 等于 d"的选法里，亲密程度 A+B 的最大值
int dp[lim*2],ndp[lim*2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>v;
	fill_n(dp,lim*2,-1145141919);
	dp[lim]=0;
	for(int i=1;i<=n;++i)
	{
		int a,b;
		cin>>a>>b;
		int dd=a-b,ss=a+b;
		fill_n(ndp,lim*2,-1145141919);
		for(int j=0;j<lim*2;++j)
		{
			if(dp[j]==-1145141919)
				continue;
			//不选第 i 件事
			ndp[j]=max(ndp[j],dp[j]);
			//选第 i 件事
			int t=j+dd;
			if(t>=0&&t<lim*2)
				ndp[t]=max(ndp[t],dp[j]+ss);
		}
		copy_n(ndp,lim*2,dp);
	}
	int ans=-1;
	for(int d=0;d<=6000;++d)
	{
		if(dp[lim+d]>=v||dp[lim-d]>=v)
		{
			ans=d;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
