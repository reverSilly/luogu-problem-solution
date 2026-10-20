//
// 题目：P1575 正误问题
// https://www.luogu.com.cn/problem/P1575
// AC
#include<bits/stdc++.h>
using namespace std;

vector<string>tk;      // 按空白切分出的记号
int pos;               // 当前记号下标
bool ok;               // 语法是否仍然合法

bool fac()             // 因子: not fac | true | false
{
	if(pos<(int)tk.size()&&tk[pos]=="not")
	{
		++pos;
		return !fac();
	}
	if(pos<(int)tk.size()&&(tk[pos]=="true"||tk[pos]=="false"))
	{
		bool v=tk[pos]=="true";
		++pos;
		return v;
	}
	ok=false;
	return false;
}
bool term()            // 项: fac (and fac)*
{
	bool v=fac();
	while(ok&&pos<(int)tk.size()&&tk[pos]=="and")
	{
		++pos;
		bool w=fac();
		v=v&&w;
	}
	return v;
}
bool expr()            // 表达式: term (or term)*
{
	bool v=term();
	while(ok&&pos<(int)tk.size()&&tk[pos]=="or")
	{
		++pos;
		bool w=term();
		v=v||w;
	}
	return v;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	while(cin>>s)
		tk.push_back(s);
	ok=true;
	pos=0;
	bool v=expr();
	if(ok&&pos==(int)tk.size())    // 合法且记号被完整消费
		cout<<(v?"true\n":"false\n");
	else
		cout<<"error\n";
	return 0;
}
