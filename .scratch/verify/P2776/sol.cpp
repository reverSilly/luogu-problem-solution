// 题目：P2776 [SDOI2007] 小组队列
// 思路：两级队列（team queue）。外层 Q 按"小组首次有人"的顺序存组号，
//       内层 g[t] 是小组 t 内部元素的 FIFO。
//       push：元素进 g[t] 尾；g[t] 由空变非空时才把 t 挂到 Q 尾。
//       pop：取 Q 头小组的 g 头元素；g 变空则把组号从 Q 头摘掉。
//       不变量：整个队列 = 按 Q 顺序拼接各 g[t]，每段非空、组号互异。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005},mxm{305};

int n,m,T,belong[mxn];
queue<int>Q,g[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>belong[i];		// 元素与小组题目已 0-based，无需 --
	cin>>T;
	for(int x,t;T--;)
	{
		string op;
		cin>>op;
		if(op=="push")
		{
			cin>>x;
			t=belong[x];
			if(g[t].empty())
				Q.push(t);		// 小组首次有人 ⇒ 排到整个队列末尾
			g[t].push(x);		// 永远接在本组最后一个元素之后
		}
		else
		{
			t=Q.front();
			x=g[t].front();
			g[t].pop();
			cout<<x<<'\n';
			if(g[t].empty())
				Q.pop();		// 小组空了 ⇒ 组号出队
		}
	}
	return 0;
}