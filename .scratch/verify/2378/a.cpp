// 题目：P2378 因式分解II
// 思路：把"日常写法"的串解析成 a*x^2+b*x+c（题面保证 a=1），
//       再用判别式 D=b^2-4c 找整数根 p>=q，使 x^2+bx+c=(x+p)(x+q)；
//       输出按题面规范：常数从大到小，(x+0) 写成 x，重根写成 ^2。
//       中间量 b^2 会超 32 位，必须 64 位 + 整数开方修正。
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

// 把当前多项式按日常写法拼回去（用于理论上不会出现的退化输入）
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

// 逐项扫描：符号 + 可选数字 + 可选 x[^2]
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
		if(!hn)					// 系数省略 ⇒ 1
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
		string str;
		for(char ch:line)
			if(!isspace((unsigned char)ch))
				str+=ch;
		if(str.empty())
			continue;
		a=b=c=0;
		parse(str);
		if(a==0)				// 退化：本来就不到二次
		{
			cout<<poly()<<'\n';
			continue;
		}
		i64 d=b*b-4*c;			// 必须 64 位
		i64 rt=isqrt(d<0?0:d);
		if(d<0||rt*rt!=d)		// 题面保证可分解，这里只是保险丝
		{
			cout<<poly()<<'\n';
			continue;
		}
		i64 p=(b+rt)/2,q=(b-rt)/2;	// p>=q，天然满足题面 a>b 的规范序
		if(p==q)
			cout<<term(p)<<"^2\n";
		else
			cout<<term(p)<<term(q)<<'\n';
	}
	return 0;
}
