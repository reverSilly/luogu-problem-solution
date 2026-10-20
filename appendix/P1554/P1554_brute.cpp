//
// 题目：P1554 [USACO06DEC] 梦中的统计 Dream Counting B ——朴素暴力基准
// https://www.luogu.com.cn/problem/P1554
// 结构独立于标程: 直接把每个数转成字符串逐字符统计
#include<bits/stdc++.h>
using namespace std;

int cnt[12];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int m,n;
	cin>>m>>n;
	for(int v=m;v<=n;++v)
	{
		string s=to_string(v);
		for(char c:s)
			++cnt[c-'0'];
	}
	for(int d=0;d<10;++d)
		cout<<cnt[d]<<(d==9?'\n':' ');
	return 0;
}
