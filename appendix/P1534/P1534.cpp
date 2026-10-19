//
// 题目：P1534 不高兴的津津（升级版）
// https://www.luogu.com.cn/problem/P1534
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	i64 cur{0},ans{0};        // cur=今天的不高兴程度, ans=逐日程度之和
	for(int i=0,a,b;i<n;++i)
	{
		cin>>a>>b;
		cur+=a+b-8;           // 今天 = 昨天 + 今日上课总时长 - 8
		ans+=cur;             // 把今天的程度计入总和
	}
	cout<<ans<<'\n';
	return 0;
}
