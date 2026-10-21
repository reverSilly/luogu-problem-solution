//
// 题目：P1851 好朋友 暴力（试除求真约数和）
// https://www.luogu.com.cn/problem/P1851
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int spd(int x)
{
	if(x<=1)
		return 0;
	int s=1;
	for(int i=2;(i64)i*i<=x;++i)
	{
		if(x%i==0)
		{
			s+=i;
			if(i!=x/i)
				s+=x/i;
		}
	}
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int s;
	cin>>s;
	for(int a=s;;++a)
	{
		int b=spd(a);
		if(b>0&&b!=a&&spd(b)==a)
		{
			cout<<a<<' '<<b<<'\n';
			return 0;
		}
	}
	return 0;
}
