// 题目：P2859 [USACO06FEB] Stall Reservations S
// 思路：区间划分。按开始时间排序 + 小根堆维护每个棚的最后结束时间。
//       开第 k 个棚时，k 个区间共同覆盖当前开始时刻 ⇒ 棚数 = 最大深度 = 最优。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n,ans[mxn];
struct cow{int a,b,i;}c[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>c[i].a>>c[i].b;
		c[i].i=i;
	}
	sort(c,c+n,[](const cow&x,const cow&y){return x.a<y.a;});
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>Q;	// (棚的最后结束时间,棚编号)
	int cnt=0;
	for(int k=0;k<n;++k)
	{
		auto[a,b,i]=c[k];
		if(!Q.empty()&&Q.top().first<a)	// 闭区间：end==a 仍冲突，必须严格 <
		{
			auto[e,s]=Q.top();Q.pop();
			ans[i]=s;
			Q.emplace(b,s);
		}
		else
		{
			ans[i]=cnt;
			Q.emplace(b,cnt);
			++cnt;
		}
	}
	cout<<cnt<<'\n';
	for(int i=0;i<n;++i)
		cout<<ans[i]+1<<'\n';
	return 0;
}
