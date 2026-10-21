//
// 题目：P1651 塔
// https://www.luogu.com.cn/problem/P1651
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxs{500005};
constexpr int inf{1145141919};

int n,h[55],dp[mxs],ndp[mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int sum=0;
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		sum+=h[i];
	}

	int lim=sum/2;
	fill_n(dp,lim+2,-inf);
	dp[0]=0;

	int cur=0;
	for(int i=0;i<n;++i)
	{
		int x=h[i];
		int ncur=min(lim,cur+x);
		for(int d=0;d<=ncur;++d)
			ndp[d]=dp[d];
		for(int d=0;d<=cur;++d)
		{
			int v=dp[d];
			if(v<0)
				continue;
			if(d+x<=lim)
				ndp[d+x]=max(ndp[d+x],v+x);
			if(x<=d)
				ndp[d-x]=max(ndp[d-x],v);
			else if(x-d<=lim)
				ndp[x-d]=max(ndp[x-d],v-d+x);
		}
		cur=ncur;
		for(int d=0;d<=cur;++d)
			dp[d]=ndp[d];
	}

	if(dp[0]<=0)
		cout<<-1<<'\n';
	else
		cout<<dp[0]<<'\n';
	return 0;
}
