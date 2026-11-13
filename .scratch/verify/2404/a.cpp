// 题目：P2404 自然数的拆分问题
// 思路：拆法内部不下降 ⇒ 每个多重集只生成一次；同层升序枚举 ⇒ DFS 叶子序就是字典序。
//       三参数 dfs(rem,st,dep)：剩余 / 本位下界 / 已填位数。
//       唯一要排除的是 dep==0 且 i==n（题面要求每项都 < n）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};

int n,a[mxn];

void prt(int len)
{
	for(int i=0;i<len;++i)
	{
		if(i)
			cout<<'+';
		cout<<a[i];
	}
	cout<<'\n';
}

// rem：还剩下多少要拆；st：本位最小可填的数；dep：已经填了几位
void dfs(int rem,int st,int dep)
{
	for(int i=st;i<=rem;++i)
	{
		if(dep==0&&i==n)			// 整个 n 单独一项不合法（每项都要 < n）
			continue;
		a[dep]=i;
		if(rem-i==0)
			prt(dep+1);
		else
			dfs(rem-i,i,dep+1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	dfs(n,1,0);
	return 0;
}
