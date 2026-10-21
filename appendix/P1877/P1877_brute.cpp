//
// 题目：P1877 [HAOI2012] 音量调节
// https://www.luogu.com.cn/problem/P1877
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};

int c[mxn],n,cap,ans;

void dfs(int i,int v)
{
	if(i==n)
	{
		ans=max(ans,v);
		return;
	}
	if(v+c[i]<=cap)
		dfs(i+1,v+c[i]);
	if(v>=c[i])
		dfs(i+1,v-c[i]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int beg;
	cin>>n>>beg>>cap;
	for(int i=0;i<n;++i)
		cin>>c[i];
	ans=-1;
	dfs(0,beg);
	cout<<ans<<'\n';
	return 0;
}
