//
// 题目：P1633 二进制
// 暴力基准：枚举 1 <= X, Y < 2^L 检验 popcount(X)=ca, popcount(Y)=cb, popcount(X+Y)=cc 且 X+Y < 2^L
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		i64 A,B,C;
		cin>>A>>B>>C;
		int L=max({__lg(A),__lg(B),__lg(C)})+1;
		int ca=__builtin_popcountll(A);
		int cb=__builtin_popcountll(B);
		int cc=__builtin_popcountll(C);
		i64 lim=1LL<<L,ans=-1;

		vector<i64>xs,ys;
		for(i64 x=1;x<lim;++x)
		{
			if(__builtin_popcountll(x)==ca)
				xs.push_back(x);
			if(__builtin_popcountll(x)==cb)
				ys.push_back(x);
		}
		for(i64 x:xs)
			for(i64 y:ys)
			{
				i64 z=x+y;
				if(z<lim&&__builtin_popcountll(z)==cc)
					if(ans==-1||z<ans)
						ans=z;
			}
		cout<<ans<<'\n';
	}
	return 0;
}
