// 题目：P2550 [AHOI2001] 彩票摇奖
// 每张彩票统计有几个号码命中中奖号码，按命中数 k 累加到 cnt[k]
// 特等奖=k7，一等奖=k6，…，六等奖=k1；依次输出 cnt[7..1]
#include<bits/stdc++.h>
using namespace std;

int n,cnt[8];
bool win[34];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,x;i<7;++i)
	{
		cin>>x;
		win[x]=true;			// 中奖号码打标记，1..33
	}
	for(int i=0;i<n;++i)
	{
		int hit=0;
		for(int j=0,x;j<7;++j)
		{
			cin>>x;
			hit+=win[x];		// 号码互不相同，直接数命中个数
		}
		++cnt[hit];
	}
	for(int k=7;k>=1;--k)
		cout<<cnt[k]<<(k==1?'\n':' ');
	return 0;
}