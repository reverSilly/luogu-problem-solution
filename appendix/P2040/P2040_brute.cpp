// 题目：P2040 打开所有的灯
// 暴力：点击两次等于没点，所以最优解中每个位置至多点击一次，
// 枚举 2^9 个点击子集，统计每个子集点亮全部灯所需的步数，取最小值
#include<bits/stdc++.h>
using namespace std;

int tg[9];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int st=0;
	for(int i=0;i<9;++i)
	{
		int x;
		cin>>x;
		if(x) st|=1<<i;
	}
	for(int i=0;i<9;++i)
	{
		int r=i/3,c=i%3,mask=1<<i;
		if(r>0) mask|=1<<(i-3);
		if(r<2) mask|=1<<(i+3);
		if(c>0) mask|=1<<(i-1);
		if(c<2) mask|=1<<(i+1);
		tg[i]=mask;
	}
	int ans=100;
	for(int s=0;s<(1<<9);++s)
	{
		int cur=st;
		for(int i=0;i<9;++i)
			if(s>>i&1) cur^=tg[i];
		if(cur==(1<<9)-1)
			ans=min(ans,__builtin_popcount((unsigned)s));
	}
	if(ans==100)
		cout<<-1<<'\n';
	else
		cout<<ans<<'\n';
	return 0;
}
