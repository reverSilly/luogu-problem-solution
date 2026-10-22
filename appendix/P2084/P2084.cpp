// 题目：P2084 进制转换
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int m;
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>s;
	int len=(int)s.size();
	bool first=true;
	for(int i=0;i<len;++i)
	{
		if(s[i]=='0')
			continue;                       // 系数为 0 的单项式省略
		if(!first)
			cout<<'+';
		first=false;
		cout<<s[i]<<'*'<<m<<'^'<<len-1-i;
	}
	cout<<'\n';
	return 0;
}
