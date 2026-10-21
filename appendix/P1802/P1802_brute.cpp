//
// 题目：P1802 5 倍经验日
// https://www.luogu.com.cn/problem/P1802
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};

int n,x;
int lose[mxn],win[mxn],use[mxn];
i64 ans;

void dfs(int i,int pot,i64 exp)
{
	if(i==n)
	{
		ans=max(ans,exp);
		return;
	}
	dfs(i+1,pot,exp+lose[i]);
	if(use[i]<=pot)
		dfs(i+1,pot-use[i],exp+win[i]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>x;
	for(int i=0;i<n;++i)
		cin>>lose[i]>>win[i]>>use[i];
	ans=0;
	dfs(0,x,0);
	cout<<ans*5<<'\n';
	return 0;
}
