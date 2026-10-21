//
// 题目：P1575 正误问题
// https://www.luogu.com.cn/problem/P1575
// AC
#include<bits/stdc++.h>
using namespace std;

vector<string> tok;
int pos_idx;
bool has_err;

bool parse_not()
{
	if(pos_idx>=static_cast<int>(tok.size()))
	{
		has_err=true;
		return false;
	}
	if(tok[pos_idx]=="not")
	{
		++pos_idx;
		return !parse_not();
	}
	if(tok[pos_idx]=="true")
	{
		++pos_idx;
		return true;
	}
	if(tok[pos_idx]=="false")
	{
		++pos_idx;
		return false;
	}
	has_err=true;
	return false;
}

bool parse_and()
{
	bool val=parse_not();
	while(!has_err&&pos_idx<static_cast<int>(tok.size())&&tok[pos_idx]=="and")
	{
		++pos_idx;
		bool rhs=parse_not();
		val=val&&rhs;
	}
	return val;
}

bool parse_or()
{
	bool val=parse_and();
	while(!has_err&&pos_idx<static_cast<int>(tok.size())&&tok[pos_idx]=="or")
	{
		++pos_idx;
		bool rhs=parse_and();
		val=val||rhs;
	}
	return val;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string w;
	while(cin>>w)
		tok.push_back(w);

	if(tok.empty())
	{
		cout<<"error\n";
		return 0;
	}

	pos_idx=0;
	has_err=false;
	bool res=parse_or();
	if(has_err||pos_idx!=static_cast<int>(tok.size()))
		cout<<"error\n";
	else
		cout<<(res?"true\n":"false\n");
	return 0;
}
