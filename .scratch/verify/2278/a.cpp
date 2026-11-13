// 题目：P2278 [HNOI2003] 操作系统
// 事件驱动模拟：两次到达之间没人能抢占，直接让堆顶跑到"结束"或"下一到达"
// 堆顶 = 优先级最高、同优先级到达最早；严格更高才抢占，同优先级自然留任
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr i64 inf{1145141919810114514LL};   // 必须大于最大可能总时间

struct proc
{
	int id;
	i64 at,rem;
	int pr;
	friend bool operator<(const proc&a,const proc&b)
	{
		if(a.pr!=b.pr)
			return a.pr<b.pr;          // 优先级大的先
		if(a.at!=b.at)
			return a.at>b.at;          // 同优先级到达早的先
		return a.id>b.id;
	}
}P[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n=0,id,pr;
	i64 at,du;
	while(cin>>id>>at>>du>>pr)
		P[n++]={id,at,du,pr};

	priority_queue<proc>Q;
	int idx=0;
	i64 t=0;
	while(idx<n||!Q.empty())
	{
		if(Q.empty())                  // CPU 空闲，时间跳到下一到达
			t=max(t,P[idx].at);
		while(idx<n&&P[idx].at<=t)     // 所有已到达的进程入堆
			Q.push(P[idx++]);
		proc u=Q.top();
		Q.pop();
		i64 nxt=(idx<n?P[idx].at:inf);
		if(t+u.rem<=nxt)               // 在下一到达前（或恰好同时）跑完
		{
			t+=u.rem;
			cout<<u.id<<' '<<t<<'\n';
		}
		else                           // 只跑到下一到达，被抢占或继续竞争
		{
			u.rem-=nxt-t;
			t=nxt;
			Q.push(u);
		}
	}
	return 0;
}
