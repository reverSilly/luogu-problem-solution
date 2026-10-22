// 题目：P2029 跳舞 —— 暴力：枚举踏中/不踏中 2^n 种方案
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,T,s[mxn],b[mxn];
int ans=-1145141919;

void dfs(int i,int cnt,int sc)
{
	if(i>n)
	{
		ans=max(ans,sc);
		return;
	}
	dfs(i+1,cnt,sc-s[i]);			// 不踏中
	int nc=cnt+1,ns=sc+s[i];
	if(nc==T) nc=0,ns+=b[i];		// 踏中，触顶则清零并领奖
	dfs(i+1,nc,ns);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>T;
	for(int i=1;i<=n;++i) cin>>s[i];
	for(int i=1;i<=n;++i) cin>>b[i];
	dfs(1,0,0);
	cout<<ans<<'\n';
	return 0;
}
