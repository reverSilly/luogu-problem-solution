// 题目：P2622 关灯问题 II
// 思路：按钮效果只依赖当前灯状态 ⇒ 状态 = 灯的子集（2^n ≤ 1024）。
//       预处理 off[i]/on[i] 位掩码，按按钮 i 的转移是 O(1)：s → (s&~off[i])|on[i]。
//       全亮掩码到 0 的无权最短路 ⇒ BFS；dis 必须显式填 INF（全局默认 0 是陷阱）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15},mxm{105},mxs{1030},inf{1145141919};

int n,m,off[mxm],on[mxm],dis[mxs];
queue<int>Q;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
		for(int j=0,x;j<n;++j)
		{
			cin>>x;
			if(x==1)
				off[i]|=1<<j;		// 1：开 → 关（强制清位）
			else if(x==-1)
				on[i]|=1<<j;		// -1：关 → 开（强制置位）
		}
	fill_n(dis,1<<n,inf);
	int st=(1<<n)-1;			// 初始全亮
	dis[st]=0;
	Q.push(st);
	while(!Q.empty())
	{
		int s=Q.front();Q.pop();
		for(int i=0;i<m;++i)
		{
			int t=(s&~off[i])|on[i];
			if(dis[t]==inf)
			{
				dis[t]=dis[s]+1;
				Q.push(t);
			}
		}
	}
	cout<<(dis[0]==inf?-1:dis[0])<<'\n';
	return 0;
}
