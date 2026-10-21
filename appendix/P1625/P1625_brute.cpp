//
// 题目：P1625 求和 （暴力对拍用，适用于 n+m 很小的数据）
// 逐项累加分数，每步用 gcd 约分，__int128 防溢出
#include<bits/stdc++.h>
using namespace std;

__int128 num=0,den=1;

__int128 gcd128(__int128 a,__int128 b)
{
	while(b)
	{
		__int128 t=a%b;
		a=b;
		b=t;
	}
	return a;
}

string s128(__int128 x)
{
	if(x==0)
		return "0";
	string s;
	while(x)
	{
		s+=(char)('0'+(int)(x%10));
		x/=10;
	}
	reverse(s.begin(),s.end());
	return s;
}

int main()
{
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;++i)
	{
		__int128 p=1;
		for(int j=i;j<=i+m-1;++j)
			p*=j;
		num=num*p+den;
		den=den*p;
		__int128 g=gcd128(num,den);
		num/=g;
		den/=g;
	}
	cout<<s128(num)<<'\n'<<s128(den)<<'\n';
	return 0;
}
