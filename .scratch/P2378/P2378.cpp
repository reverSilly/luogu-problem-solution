#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 a,b,c;			// a*x^2+b*x+c，题面保证 a=1

// 把 "(x+k)" 写成日常形式：k=0 时就是 "x"
string term(i64 k)
{
	if(k==0)
		return "x";
	if(k>0)
		return "(x+"+to_string(k)+")";
	return "(x-"+to_string(-k)+")";
}

// 把当前多项式按日常写法打印（用于理论上不会出现的「不可分解」兜底）
string poly()
{
	string r;
	if(a!=0)
	{
		if(a==1)
			r+="x^2";
		else if(a==-1)
			r+="-x^2";
		else
			r+=to_string(a)+"x^2";
	}
	if(b!=0)
	{
		if(!r.empty()&&b>0)
			r+="+";
		if(b==1)
			r+="x";
		else if(b==-1)
			r+="-x";
		else
			r+=to_string(b)+"x";
	}
	if(c!=0)
	{
		if(!r.empty()&&c>0)
			r+="+";
		r+=to_string(c);
	}
	if(r.empty())
		r="0";
	return r;
}

i64 isqrt(i64 x)
{
	i64 s=(i64)sqrtl((long double)x);
	while(s>0&&s*s>x)
		--s;
	while((s+1)*(s+1)<=x)
		++s;
	return s;
}

void parse(const string&s)
{
	int n=s.size(),i=0,sg=1;
	if(i<n&&(s[i]=='-'||s[i]=='+'))
	{
		if(s[i]=='-')
			sg=-1;
		++i;
	}
	while(i<n)
	{
		i64 coef=0;
		bool hn=false;
		while(i<n&&isdigit((unsigned char)s[i]))
		{
			coef=coef*10+(s[i]-'0');
			hn=true;
			++i;
		}
		if(!hn)
			coef=1;
		if(i<n&&s[i]=='x')
		{
			++i;
			int e=1;
			if(i<n&&s[i]=='^')
			{
				++i;e=0;
				while(i<n&&isdigit((unsigned char)s[i]))
				{
					e=e*10+(s[i]-'0');
					++i;
				}
			}
			if(e==2)
				a+=coef*sg;
			else if(e==1)
				b+=coef*sg;
		}
		else
			c+=coef*sg;
		if(i<n)
		{
			if(s[i]=='-')
				sg=-1;
			else
				sg=1;
			++i;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	while(getline(cin,line))
	{
		string s;
		for(char ch:line)
			if(!isspace((unsigned char)ch))
				s+=ch;
		if(s.empty())
			continue;
		a=b=c=0;
		parse(s);
		if(a==0)
		{
			// 次数不超过 1（理论上题目不给，兜底按日常写法原样输出）
			cout<<poly()<<'\n';
			continue;
		}
		// a==1：x^2+bx+c=(x+p)(x+q)，p+q=b，pq=c
		i64 d=b*b-4*c;
		i64 rt=isqrt(d<0?0:d);
		if(d<0||rt*rt!=d)
		{
			cout<<poly()<<'\n';
			continue;
		}
		i64 p=(b+rt)/2,q=(b-rt)/2;		// p>=q，天然满足题面 a>b 的规范
		if(p==q)
			cout<<term(p)<<"^2\n";
		else
			cout<<term(p)<<term(q)<<'\n';
	}
	return 0;
}
