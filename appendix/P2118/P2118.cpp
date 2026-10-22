// 题目：P2118 [NOIP 2014 普及组] 比例简化
#include<bits/stdc++.h>
using namespace std;

using i64=long long;

int A,B,L;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B>>L;

	int ba=0,bb=0;
	for(int a=1;a<=L;++a)
		for(int b=1;b<=L;++b)
		{
			if(gcd(a,b)!=1) continue;
			if(1LL*a*B<1LL*A*b) continue;
			if(ba==0 || 1LL*a*bb<1LL*ba*b)
			{
				ba=a;
				bb=b;
			}
		}
	cout<<ba<<' '<<bb<<'\n';
	return 0;
}
