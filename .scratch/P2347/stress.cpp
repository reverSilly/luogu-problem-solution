#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
const int w[6]={1,2,3,5,10,20};

int a[6];
bool dp[mxn];

int fast(const array<int,6>&c)
{
	memset(dp,0,sizeof(dp));
	int S=0;
	for(int i=0;i<6;++i)
	{
		a[i]=c[i];
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
	return ans;
}

bool vis[100005];

void dfs(int i,int s,const array<int,6>&c)
{
	if(i==6)
	{
		vis[s]=true;
		return;
	}
	for(int k=0;k<=c[i];++k)
		dfs(i+1,s+k*w[i],c);
}

int brute(const array<int,6>&c)
{
	memset(vis,0,sizeof(vis));
	dfs(0,0,c);
	int ans=0;
	for(int s=1;s<100005;++s)
		if(vis[s])
			++ans;
	return ans;
}

int main()
{
	mt19937 rn(20241022);
	for(int t=1;t<=1500;++t)
	{
		array<int,6>c;
		do
		{
			for(int i=0;i<6;++i)
				c[i]=rn()%4;
		}while([&]{
			int S=0;
			for(int i=0;i<6;++i)
				S+=c[i]*w[i];
			return S>60;
		}());
		int r1=fast(c),r2=brute(c);
		if(r1!=r2)
		{
			cerr<<"Mismatch at test "<<t<<": fast="<<r1<<" brute="<<r2<<'\n';
			for(int i=0;i<6;++i)
				cerr<<c[i]<<' ';
			cerr<<'\n';
			return 1;
		}
	}
	cout<<"all 1500 tests passed\n";
	return 0;
}
