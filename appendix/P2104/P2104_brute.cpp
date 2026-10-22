#include<bits/stdc++.h>
using namespace std;

string s,op;

// 加 1: 找到最低位侧第一个 0, 把它改成 1, 途经的 1 全变 0
void add_one()
{
	int i=(int)s.size()-1;
	while(i>=0 && s[i]=='1')
	{
		s[i]='0';
		--i;
	}
	if(i>=0) s[i]='1';
	else s="1"+string(s.size(),'0');	// 最高位进位(题目保证不会出现)
}

// 减 1: 找到最低位侧第一个 1, 把它改成 0, 途经的 0 全变 1
void sub_one()
{
	int i=(int)s.size()-1;
	while(i>=0 && s[i]=='0')
	{
		s[i]='1';
		--i;
	}
	if(i>=0) s[i]='0';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	cin>>s>>op;

	for(char c:op)
	{
		if(c=='+') add_one();
		else if(c=='-') sub_one();
		else if(c=='*') s+='0';
		else if(c=='/')
		{
			if(!s.empty()) s.pop_back();
		}
	}
	cout<<s<<'\n';
	return 0;
}
