// 题目：P2062 分队问题
// 暴力：状态压缩 DP，把选手集合划分成若干合法队伍，求最多队伍数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{21};

int n,a[mxn];
int f[1<<11];				// f[mask]: 把 mask 中的选手分队后最多的队伍数，-1 表示无合法划分
int mx[1<<11];				// mx[mask]: mask 中 a 的最大值

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i) cin>>a[i];
	int lim=1<<n;
	for(int s=1;s<lim;++s) f[s]=-1;
	f[0]=0;
	for(int s=1;s<lim;++s)
	{
		int i=__builtin_ctz((unsigned)s);
		mx[s]=max(mx[s^(1<<i)],a[i]);
	}
	for(int s=1;s<lim;++s)
	{
		// 枚举包含最低位选手的子集作为一支队伍
		int low=s&-s;
		int rest=s^low;
		for(int t=rest;;t=(t-1)&rest)
		{
			int team=t|low;
			if(__builtin_popcount((unsigned)team)>=mx[team]&&f[s^team]>=0)
				f[s]=max(f[s],f[s^team]+1);
			if(t==0) break;
		}
	}
	cout<<f[lim-1]<<'\n';
	return 0;
}
