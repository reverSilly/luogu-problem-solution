// 题目：P2887 [USACO07NOV] Sunscreen G
// 防晒霜按 SPF 从小到大扫；lo<=SPF 的牛入小根堆（键 hi）。
// 先弹掉 hi<SPF 的过期牛（之后 SPF 只会更大，永远没救），
// 再把 cover 瓶分给堆顶（hi 最小）的牛：上限紧的牛先救，选择余地大的牛留给后面。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2505};

int c,l;
struct{int lo,hi;}cow[mxn];
struct{int spf,cov;}bot[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>c>>l;
	for(int i=0;i<c;++i)
		cin>>cow[i].lo>>cow[i].hi;
	for(int i=0;i<l;++i)
		cin>>bot[i].spf>>bot[i].cov;
	sort(cow,cow+c,[](auto a,auto b){return a.lo<b.lo;});
	sort(bot,bot+l,[](auto a,auto b){return a.spf<b.spf;});
	priority_queue<int,vector<int>,greater<>>Q;	// 未分配且 lo 已过界的牛，按 hi 小根堆
	int j=0,ans=0;
	for(int i=0;i<l;++i)
	{
		int s=bot[i].spf;
		while(j<c&&cow[j].lo<=s)
			Q.push(cow[j++].hi);
		while(!Q.empty()&&Q.top()<s)	// 过期牛：任何剩余防晒霜都救不了
			Q.pop();
		for(int k=0;k<bot[i].cov&&!Q.empty();++k)
		{
			Q.pop();	// 堆顶 hi>=s 且 lo<=s，恰好可用
			++ans;
		}
	}
	cout<<ans<<'\n';
	return 0;
}