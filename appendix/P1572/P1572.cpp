//
// 题目：P1572 计算分数
// https://www.luogu.com.cn/problem/P1572
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string s;

i64 gcd(i64 a,i64 b)
{
	a=a<0?-a:a;
	b=b<0?-b:b;
	while(b)
	{
		i64 t=a%b;
		a=b;
		b=t;
	}
	return a;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int i=0,len=s.size();
	i64 p=0,q=1;              // 累积分数 p/q(q>0)
	int sg=1;                 // 当前项符号
	if(s[0]=='-')
	{
		sg=-1;
		i=1;
	}
	while(i<len)
	{
		i64 a=0,b=0;
		while(i<len&&s[i]>='0'&&s[i]<='9')
			a=a*10+(s[i++]-'0');
		++i;                  // 跳过 '/'
		while(i<len&&s[i]>='0'&&s[i]<='9')
			b=b*10+(s[i++]-'0');
		p=p*b+sg*a*q;         // p/q + sg*(a/b) = (p*b+sg*a*q)/(q*b)
		q=q*b;
		i64 g=gcd(p,q);       // 每步立即约分,保证中间量在范围内
		p/=g;
		q/=g;
		if(i<len)
		{
			sg=(s[i]=='-')?-1:1;
			++i;
		}
	}
	if(q==1)
		cout<<p<<'\n';        // 整数直接输出整数
	else
		cout<<p<<'/'<<q<<'\n';
	return 0;
}
