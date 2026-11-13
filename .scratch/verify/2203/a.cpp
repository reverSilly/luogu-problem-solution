#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxst{1<<16};

int n,mask;
i64 B;
int seen[mxst];     // seen[s]=状态s第一次出现的时刻
int hist[mxst+5];   // hist[t]=时刻t的状态

int step(int s)
{
	int nb=(s<<1)|(s>>(n-1));     // 第i位取左边邻居(即i-1位,环形)
	return (s^nb)&mask;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>B;
	int s=0;
	for(int i=0;i<n;++i)
	{
		int x;
		cin>>x;
		s|=x<<i;
	}
	mask=(1<<n)-1;
	fill_n(seen,mxst,-1);
	int t=0;
	while(seen[s]==-1)
	{
		seen[s]=t;
		hist[t]=s;
		s=step(s);
		++t;
	}
	// 当前时刻t的状态等于时刻t0=seen[s]的状态，周期p=t-t0
	int t0=seen[s],p=t-t0;
	int ans;
	if(B<t)
		ans=hist[B];
	else
		ans=hist[t0+(B-t0)%p];
	for(int i=0;i<n;++i)
		cout<<((ans>>i)&1)<<'\n';
	return 0;
}
