//
// 题目：P1866 编号
// https://www.luogu.com.cn/problem/P1866
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};
constexpr int mod{1000000007};

int n;
int a[mxn];
int used[1005];
i64 ans;

// 逐只兔子枚举合法编号，与排序后的乘法公式不同
void dfs(int i)
{
	if(i==n)
	{
		ans+=1;
		if(ans>=mod)
			ans-=mod;
		return;
	}
	for(int x=1;x<=a[i];++x)
		if(!used[x])
		{
			used[x]=1;
			dfs(i+1);
			used[x]=0;
		}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mx=max(mx,a[i]);
	}
	if(mx>1000)
	{
		cout<<0<<'\n';
		return 0;
	}
	dfs(0);
	cout<<ans<<'\n';
	return 0;
}
