//
// 题目：P1554 [USACO06DEC] 梦中的统计 Dream Counting B
// https://www.luogu.com.cn/problem/P1554
// AC
#include<bits/stdc++.h>
using namespace std;

int cnt[12];                       // cnt[d]=数码 d 出现次数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int m,n;
	cin>>m>>n;
	for(int v=m;v<=n;++v)          // 区间长度 <= 5e5+1, 直接逐个数位统计
	{
		int x=v;
		if(x==0)                   // 保险分支(题面 m>=1, 实际不会走到)
			++cnt[0];
		while(x)
		{
			++cnt[x%10];
			x/=10;
		}
	}
	for(int d=0;d<10;++d)
		cout<<cnt[d]<<(d==9?'\n':' ');
	return 0;
}
