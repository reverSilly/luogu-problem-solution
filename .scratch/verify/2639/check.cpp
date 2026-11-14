#include<bits/stdc++.h>
using namespace std;

int dp[45005];
int solve(int H,vector<int>w)
{
	fill(dp,dp+H+1,0);
	for(int x:w)for(int j=H;j>=x;--j)dp[j]=max(dp[j],dp[j-x]+x);
	return dp[H];
}
int brute(int H,vector<int>w)
{
	int n=w.size(),best=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int s=0;for(int i=0;i<n;++i)if(mask>>i&1)s+=w[i];
		if(s<=H)best=max(best,s);
	}
	return best;
}
int main()
{
	mt19937 rn(42);int bad=0;
	for(int t=0;t<20000;++t)
	{
		int H=5+rn()%56,n=1+rn()%14;vector<int>w(n);
		for(int&x:w)x=1+rn()%H;
		int a=solve(H,w),b=brute(H,w);
		if(a!=b){++bad;if(bad<=5)cerr<<"bad H="<<H<<" a="<<a<<" b="<<b<<"\n";}
	}
	cerr<<"mismatch="<<bad<<"\n";
	return 0;
}