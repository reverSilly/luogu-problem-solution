//
// 题目：P1553 数字反转（升级版）——朴素暴力基准
// https://www.luogu.com.cn/problem/P1553
// 结构独立于标程: 先按分隔符切分, 再分别用 find_first_not_of / find_last_not_of 去零
#include<bits/stdc++.h>
using namespace std;

string stripLead(string s)   // 去前导 0, 全 0 则返回 "0"
{
	size_t p=s.find_first_not_of('0');
	return p==string::npos?"0":s.substr(p);
}
string stripTail(string s)   // 去末尾 0, 全 0 则返回 "0"
{
	size_t p=s.find_last_not_of('0');
	return p==string::npos?"0":s.substr(0,p+1);
}
string revInt(string s)      // 整数型片段
{
	reverse(s.begin(),s.end());
	return stripLead(s);
}
string revDec(string s)      // 小数型片段
{
	reverse(s.begin(),s.end());
	return stripTail(s);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	if(s.back()=='%')                       // 百分数
		cout<<revInt(s.substr(0,s.size()-1))<<"%\n";
	else if(s.find('.')!=string::npos)      // 小数
	{
		size_t p=s.find('.');
		cout<<revInt(s.substr(0,p))<<'.'<<revDec(s.substr(p+1))<<'\n';
	}
	else if(s.find('/')!=string::npos)      // 分数
	{
		size_t p=s.find('/');
		cout<<revInt(s.substr(0,p))<<'/'<<revInt(s.substr(p+1))<<'\n';
	}
	else                                     // 整数
		cout<<revInt(s)<<'\n';
	return 0;
}
