//
// 题目：P1626 象棋比赛 （暴力对拍用）
// 枚举所有 K 元对局集合，检查每人白/黑各至多用一次，取等级差和最小
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,k,a[10];
int eu[100],ev[100],ec;
int usedw,usedb,cur;
i64 best;

void dfs(int p,int left)
{
	if(left==0)
	{
		best=min(best,(i64)cur);
		return;
	}
	if(p>=ec||ec-p<left)
		return;
	for(int i=p;i<ec;++i)
	{
		int u=eu[i],v=ev[i];             // a[u]<a[v]，u 白 v 黑
		if((usedw>>u&1)||(usedb>>v&1))
			continue;
		usedw|=1<<u;
		usedb|=1<<v;
		cur+=a[v]-a[u];
		dfs(i+1,left-1);
		cur-=a[v]-a[u];
		usedw^=1<<u;
		usedb^=1<<v;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			if(a[i]<a[j])
			{
				eu[ec]=i;ev[ec]=j;++ec;
			}
			else
			{
				eu[ec]=j;ev[ec]=i;++ec;
			}
	best=(i64)4e18;
	dfs(0,k);
	cout<<best<<'\n';
	return 0;
}
