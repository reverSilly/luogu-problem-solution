// 暴力基准:先把所有项收进列表,再用"公共分母=各分母之积"一次性求和,
// 与标程的"逐步通分+立即约分"是完全不同的算术路径;用 __int128 防中间溢出。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128;

struct term{int sg;i64 a;i64 b;};

vector<term> T;

i128 gcd128(i128 a,i128 b)
{
	if(a<0)
		a=-a;
	if(b<0)
		b=-b;
	while(b)
	{
		i128 t=a%b;
		a=b;
		b=t;
	}
	return a;
}

void print128(i128 x)
{
	if(x<0)
	{
		cout<<'-';
		x=-x;
	}
	if(x==0)
	{
		cout<<'0';
		return;
	}
	string t;
	while(x)
	{
		t+=char('0'+x%10);
		x/=10;
	}
	reverse(t.begin(),t.end());
	cout<<t;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	cin>>s;
	int i=0,len=s.size();
	int sg=1;
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
		++i;
		while(i<len&&s[i]>='0'&&s[i]<='9')
			b=b*10+(s[i++]-'0');
		T.push_back({sg,a,b});
		if(i<len)
		{
			sg=(s[i]=='-')?-1:1;
			++i;
		}
	}

	i128 D=1;
	for(auto&t:T)
		D*=t.b;
	i128 num=0;
	for(auto&t:T)
		num+=i128(t.sg)*t.a*(D/t.b);

	i128 g=gcd128(num,D);
	if(g!=0)
	{
		num/=g;
		D/=g;
	}
	if(D==1)
		print128(num);
	else
	{
		print128(num);
		cout<<'/';
		print128(D);
	}
	cout<<'\n';
	return 0;
}
