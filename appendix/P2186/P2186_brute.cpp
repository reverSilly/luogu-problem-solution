// P2186 小Z的栈函数
// 暴力：使用 vector 逐条重放操作，用字符串表匹配操作名，与标程的实现方式相互独立
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

const long long LIM=1000000000LL;

vector<pair<string,i64>> ops;

i64 eval(i64 x)
{
	vector<i64> s;
	s.push_back(x);
	for(size_t i=0;i<ops.size();++i)
	{
		string op=ops[i].first;
		i64 arg=ops[i].second;
		if(op=="NUM")
		{
			if(arg>1000000000LL||arg<-1000000000LL)
				return 1145141919;
			s.push_back(arg);
		}
		else if(op=="POP")
		{
			if(s.empty())
				return 1145141919;
			s.pop_back();
		}
		else if(op=="INV")
		{
			if(s.empty())
				return 1145141919;
			s.back()=-s.back();
		}
		else if(op=="DUP")
		{
			if(s.empty())
				return 1145141919;
			s.push_back(s.back());
		}
		else if(op=="SWP")
		{
			if(s.size()<2)
				return 1145141919;
			swap(s[s.size()-1],s[s.size()-2]);
		}
		else if(op=="ADD"||op=="SUB"||op=="MUL"||op=="DIV"||op=="MOD")
		{
			if(s.size()<2)
				return 1145141919;
			i64 a=s.back();
			s.pop_back();
			i64 b=s.back();
			s.pop_back();
			i64 r=0;
			if(op=="ADD")
				r=a+b;
			else if(op=="SUB")
				r=b-a;
			else if(op=="MUL")
				r=a*b;
			else if(op=="DIV")
			{
				if(a==0)
					return 1145141919;
				r=b/a;
			}
			else
			{
				if(a==0)
					return 1145141919;
				r=b%a;
			}
			if(r>1000000000LL||r<-1000000000LL)
				return 1145141919;
			s.push_back(r);
		}
		else
			return 1145141919;
	}
	if(s.size()!=1)
		return 1145141919;
	return s[0];
}

int main()
{
	string tok;
	while(cin>>tok)
	{
		if(tok=="END")
			break;
		i64 v=0;
		if(tok=="NUM")
			cin>>v;
		ops.push_back(make_pair(tok,v));
	}
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		i64 x;
		cin>>x;
		i64 r=eval(x);
		if(r==1145141919)
			printf("ERROR\n");
		else
			printf("%lld\n",r);
	}
	return 0;
}
