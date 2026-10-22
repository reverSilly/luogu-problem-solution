// 题目：P2118 比例简化 —— 暴力: 直接比较差值 (a*B-A*b)/b, 用交叉相乘避免浮点误差
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
			i64 num=1LL*a*B-1LL*A*b;	// 分子 >= 0
			if(num<0) continue;
			if(ba==0) { ba=a; bb=b; continue; }
			// 比较 num/b 与 nb/bb
			i64 nb=1LL*ba*B-1LL*A*bb;
			if(num*bb<nb*b) { ba=a; bb=b; }
		}
	cout<<ba<<' '<<bb<<'\n';
	return 0;
}
