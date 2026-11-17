// 题目：P3093 [USACO13DEC] Milk Scheduling S
// 单位时长任务 + 截止时间 + 收益 ⇒ 按 d 升序扫，小根堆反悔贪心：
// 先收下当前牛，堆大小超过 d 就踢掉产量最小的（反悔）。
// 不变量：处理完前 i 头后，堆是前 i 头中收益最大的可行子集。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,ans;
struct cow{int g,d;}c[mxn];
priority_queue<int,vector<int>,greater<>>Q;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>c[i].g>>c[i].d;
	sort(c,c+n,[](cow a,cow b){return a.d<b.d;});
	for(int i=0;i<n;++i)
	{
		Q.push(c[i].g);
		ans+=c[i].g;
		if((int)Q.size()>c[i].d)	// 超容量则反悔：踢掉产量最小的
		{
			ans-=Q.top();
			Q.pop();
		}
	}
	cout<<ans<<'\n';
	return 0;
}
