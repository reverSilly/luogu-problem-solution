// 题目：P2029 跳舞
// 累计计数模型：计数 c 表示"距上次清零以来已踏中的次数"，取值 0..T-1
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{5005};
constexpr i64 inf{-1145141919};

int n,T,s[mxn],b[mxn];
i64 f[2][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>T;
	for(int i=1;i<=n;++i) cin>>s[i];
	for(int i=1;i<=n;++i) cin>>b[i];

	if(T<=0)	// T=0 时累计奖励永不触发，把所有箭头都踏中即可
	{
		i64 ans=0;
		for(int i=1;i<=n;++i) ans+=s[i];
		cout<<ans<<'\n';
		return 0;
	}

	// f[cur][c]: 已处理完前若干箭头、累计计数为 c 时的最大得分
	i64*cur=f[0],*nxt=f[1];
	fill(cur,cur+T,inf);
	cur[0]=0;
	for(int i=1;i<=n;++i)
	{
		for(int c=0;c<T;++c)
		{
			i64 best=inf;
			if(cur[c]!=inf) best=cur[c]-s[i];		// 不踏中：扣分，计数不变
			int pc=(c==0?T-1:c-1);				// 踏中转移的来源计数
			if(cur[pc]!=inf)
			{
				i64 v=cur[pc]+s[i];
				if(c==0) v+=b[i];				// 由 T-1 触顶 -> 计数清零并给 B_i
				best=max(best,v);
			}
			nxt[c]=best;
		}
		swap(cur,nxt);
	}
	i64 ans=inf;
	for(int c=0;c<T;++c) ans=max(ans,cur[c]);
	cout<<ans<<'\n';
	return 0;
}
