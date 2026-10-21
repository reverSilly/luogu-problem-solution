//
// 题目：P1654 OSU!
// 暴力基准：枚举全部 2^n 个 01 串，按定义计算每个串极长连续 1 段的立方和与精确期望
#include<bits/stdc++.h>
using namespace std;

int n;
double p[25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>p[i];

	double ans=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		double prob=1;
		int score=0,run=0;
		for(int i=0;i<n;++i)
		{
			if(mask>>i&1)
			{
				prob*=p[i];
				++run;
			}
			else
			{
				prob*=1-p[i];
				score+=run*run*run;
				run=0;
			}
		}
		score+=run*run*run;
		ans+=prob*score;
	}
	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}
