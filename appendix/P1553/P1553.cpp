//
// 题目：P1553 数字反转（升级版）
// https://www.luogu.com.cn/problem/P1553
// AC
#include<bits/stdc++.h>
using namespace std;

string revInt(string t)          // 整数型片段: 反转后去前导 0(至少留一位)
{
	reverse(t.begin(),t.end());
	int i=0;
	while(i+1<(int)t.size()&&t[i]=='0')
		++i;
	return t.substr(i);
}
string revDec(string t)          // 小数型片段: 反转后去末尾 0(至少留一位)
{
	reverse(t.begin(),t.end());
	int i=(int)t.size()-1;
	while(i>0&&t[i]=='0')
		--i;
	return t.substr(0,i+1);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	size_t dot=s.find('.');
	size_t slash=s.find('/');
	size_t pct=s.find('%');
	if(pct!=string::npos)                          // 百分数: 只反转数字部分
		cout<<revInt(s.substr(0,s.size()-1))<<"%\n";
	else if(dot!=string::npos)                     // 小数: 两段各自反转, 不交换
		cout<<revInt(s.substr(0,dot))<<'.'<<revDec(s.substr(dot+1))<<'\n';
	else if(slash!=string::npos)                   // 分数: 分子分母各自反转, 不交换
		cout<<revInt(s.substr(0,slash))<<'/'<<revInt(s.substr(slash+1))<<'\n';
	else                                           // 整数
		cout<<revInt(s)<<'\n';
	return 0;
}
