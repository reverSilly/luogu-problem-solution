// 题目：P2234 [HNOI2002] 营业额统计
// 每天在有序集合里找 a 的前驱/后继，取较近者；第一天直接加 a
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n;
set<int>s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 ans=0;
	for(int i=0,a,best;i<n;++i)
	{
		cin>>a;
		if(i==0)
		{
			ans+=a;   // 第一天的最小波动值就是当天的营业额（可为负）
			s.insert(a);
			continue;
		}
		auto it=s.lower_bound(a);   // 第一个 >= a，即后继
		best=1145141919;
		if(it!=s.end())
			best=*it-a;
		if(it!=s.begin())
			best=min(best,a-*prev(it));   // 前驱
		ans+=best;
		s.insert(a);   // 先查后插
	}
	cout<<ans<<'\n';
	return 0;
}
