//
// 题目：P1510 精卫填海 (暴力基准)
// https://www.luogu.com.cn/problem/P1510
// 仅用于小规模对拍: 枚举全部子集, 求达到体积 v 的最小体力消耗
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};
constexpr int inf{1145141919};

int v,n,c;
int k[mxn],mm[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>v>>n>>c;
	for(int i=0;i<n;++i)
		cin>>k[i]>>mm[i];

	int best=inf;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int sv=0,sc=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1)
			{
				sv+=k[i];sc+=mm[i];
			}
		if(sv>=v&&sc<=c)
			best=min(best,sc);
	}
	if(best==inf)
		cout<<"Impossible\n";
	else
		cout<<c-best<<'\n';
	return 0;
}
