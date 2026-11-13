// 题目：P2311 loidc，想想看
// 关键：x、y 都不减 => 询问是只向右滑的窗口 => 单调队列（存下标，值队头到队尾严格递减）
// 队尾弹出：右边有更大的 => 被支配，以后永远不可能当最大值
// 队头弹出：下标 < x => 已经出窗口
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m,a[mxn],Q[mxn];	//Q[hd,tl) 存下标，值从队头到队尾严格递减

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int hd=0,tl=0,cur=0;	//cur=已入队右端点（开），[0,cur)
	cin>>m;
	for(int x,y;m--;)
	{
		cin>>x>>y;--x;--y;
		for(;cur<=y;++cur)	//先推进右端点
		{
			while(hd<tl&&a[Q[tl-1]]<a[cur])
				--tl;
			Q[tl++]=cur;
		}
		while(hd<tl&&Q[hd]<x)	//再弹出出窗口的队头
			++hd;
		cout<<Q[hd]+1<<'\n';	//y>=x 保证队列非空
	}
	return 0;
}
