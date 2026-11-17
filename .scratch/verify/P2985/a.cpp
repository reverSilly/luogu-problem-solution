// 题目：P2985 [USACO10FEB] Chocolate Eating S
// 二分最低幸福值 X；贪心判定：每天起床幸福值不够 X 才按顺序吃，最后一天扫尾吃完
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{50005};

int n,d,h[mxn],day[mxn];
i64 sum;

bool check(i64 x,bool rec)
{
	i64 cur=0;	// 起床时幸福值
	int ptr=0;
	for(int t=1;t<=d;++t)
	{
		while(cur<x&&ptr<n)
		{
			cur+=h[ptr];
			if(rec)day[ptr]=t;
			++ptr;
		}
		if(cur<x)return false;	// 巧克力吃完仍不够 X
		if(t==d)
			while(ptr<n)	// 剩余的必须吃完，全塞最后一天只会更优
			{
				if(rec)day[ptr]=t;
				cur+=h[ptr++];
			}
		cur/=2;	// 睡觉减半下取整
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>d;
	for(int i=0;i<n;++i)
		cin>>h[i],sum+=h[i];
	i64 l=0,r=sum+1;	// 不变量：l 可行，r 不可行；sum 可达 5e10 必须 i64
	while(l+1<r)
	{
		i64 mid=(l+r)>>1;
		if(check(mid,false))l=mid;
		else r=mid;
	}
	check(l,true);
	cout<<l<<'\n';
	for(int i=0;i<n;++i)
		cout<<day[i]<<'\n';
	return 0;
}