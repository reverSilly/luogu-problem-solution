// P2719 暴力：枚举前 2n-2 人的所有掷硬币序列，模拟售票，统计最后两人同票的频率
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int N;cin>>N;int n=N/2;
	int steps=2*n-2;
	long long good=0,tot=1LL<<steps;
	for(long long mask=0;mask<tot;++mask)
	{
		int a=0,b=0;
		for(int s=0;s<steps;++s)
		{
			int flip=(mask>>s)&1;	// 0=A,1=B
			if(flip==0&&a<n)++a;
			else if(flip==1&&b<n)++b;
			else if(a<n)++a;	// B 售罄，强制 A
			else ++b;	// A 售罄，强制 B
		}
		// 剩 2 张票：若某类已空则最后两人同票
		if(a==n||b==n)++good;
	}
	printf("%.4f\n",(double)good/tot);
	return 0;
}