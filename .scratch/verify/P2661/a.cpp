// 题目：P2661 [NOIP 2015 提高组] 信息传递
// 思路：信息每轮沿 i→T_i 走一跳，v 在第 r 轮听到自己的生日 ⇔ T^r(v)=v。
//       答案 = 函数图（基环树森林）的最小环长。
//       全局时间戳游走：每个点只 stamp 一次；撞到本轮 stamp 的点 ⇒ 新环，
//       环长 = clk-vis[u]+1；撞到旧点 ⇒ 汇入已处理部分，无新环。O(n)。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005},inf{1145141919};

int n,t[mxn],vis[mxn],clk,ans=inf;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>t[i];--t[i];
	}
	for(int i=0;i<n;++i)
	{
		if(vis[i])
			continue;
		int st=clk,u=i;
		while(!vis[u])
		{
			vis[u]=++clk;	// 全局时间戳：每个点一生只被 stamp 一次
			u=t[u];
		}
		if(vis[u]>st)		// 撞到本轮的点 ⇒ 新环；旧点 ⇒ 环早已统计
			ans=min(ans,clk-vis[u]+1);
	}
	cout<<ans<<'\n';
	return 0;
}