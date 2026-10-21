//
// P1602 暴力: 按数值递增枚举所有"数位都 < k"的正整数, 第一个整除 m 的即答案
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int k,m;
	cin>>k>>m;

	priority_queue<i64,vector<i64>,greater<i64>>Q;
	for(int d=1;d<k;++d)
		Q.push(d);

	const i64 LIM=1000000000000000000LL;   // 1e18
	long long pops=0;
	while(!Q.empty())
	{
		i64 x=Q.top();
		Q.pop();
		if(++pops>4000000)
			break;
		if(x%m==0)
		{
			cout<<x<<'\n';
			return 0;
		}
		if(x<=LIM/10)
			for(int d=0;d<k;++d)
				Q.push(x*10+d);
	}
	cout<<"TOOBIG\n";
	return 0;
}
