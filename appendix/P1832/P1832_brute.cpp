//
// 题目：P1832 A+B Problem（再升级）
// https://www.luogu.com.cn/problem/P1832
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{85};

int n,pr[mxn],pc;
bool comp[mxn];
i64 ans;

void dfs(int idx,int rem)
{
	if(rem==0)
	{
		++ans;
		return;
	}
	for(int i=idx;i<pc;++i)
	{
		if(pr[i]>rem)
			break;
		dfs(i,rem-pr[i]);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=2;i<=n;++i)
	{
		if(!comp[i])
			pr[pc++]=i;
		for(int j=0;j<pc&&i*1LL*pr[j]<=n;++j)
		{
			comp[i*pr[j]]=true;
			if(i%pr[j]==0)
				break;
		}
	}
	if(n==0)
		ans=1;
	else
		dfs(0,n);
	cout<<ans<<'\n';
	return 0;
}
