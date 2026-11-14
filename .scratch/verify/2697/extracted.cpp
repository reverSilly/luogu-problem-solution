// 题目：P2697 宝石串
// G=+1,R=-1 ⇒ 子串稳定 ⟺ 两端前缀和相等 ⇒ 求最远等值前缀和对
// fst[v] 记录前缀和值 v 第一次出现的位置，扫右端点 j 取 j-fst[s[j]]
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005},inf{1145141919};

int n,fst[mxn*2],ans;
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	n=s.size();
	fill_n(fst,2*n+1,inf);	// 0 是合法位置，"未出现"必须显式填哨兵
	fst[n]=0;	// s[0]=0，下标偏移 +n
	int cur=0;
	for(int j=1;j<=n;++j)
	{
		cur+=s[j-1]=='G'?1:-1;
		int v=cur+n;
		if(fst[v]==inf)
			fst[v]=j;
		else
			ans=max(ans,j-fst[v]);
	}
	cout<<ans<<'\n';
	return 0;
}
