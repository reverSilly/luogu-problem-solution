//
// 题目：P1647 锁
// 暴力基准：按题意贪心逐步生成序列前 K 项并维护最大值
#include<bits/stdc++.h>
using namespace std;

int n,k;
bool vis[1<<16];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	int cur=0,mx=0,lim=1<<n;
	vis[0]=true;
	for(int step=1;step<k;++step)
	{
		for(int cand=0;cand<lim;++cand)
		{
			if(vis[cand])
				continue;
			int diff=cur^cand;
			if((cur&diff)==0||(cand&diff)==0)
			{
				vis[cand]=true;
				cur=cand;
				mx=max(mx,cur);
				break;
			}
		}
	}
	for(int i=n-1;i>=0;--i)
		cout<<((mx>>i)&1);
	cout<<'\n';
	return 0;
}
