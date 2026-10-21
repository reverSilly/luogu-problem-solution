//
// 题目：P1838 三子棋I
// https://www.luogu.com.cn/problem/P1838
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{12};
constexpr int line[8][3]{
	{1,2,3},{4,5,6},{7,8,9},
	{1,4,7},{2,5,8},{3,6,9},
	{1,5,9},{3,5,7}
};

int bd[mxn];

bool win(int who)
{
	for(int i=0;i<8;++i)
	{
		int a=line[i][0],b=line[i][1],c=line[i][2];
		if(bd[a]==who&&bd[b]==who&&bd[c]==who)
			return true;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	for(int i=0;i<(int)s.size();++i)
	{
		int p=s[i]-'0';
		int who=i%2==0?1:2;
		bd[p]=who;
		if(win(who))
		{
			if(who==1)
				cout<<"xiaoa wins.\n";
			else
				cout<<"uim wins.\n";
			return 0;
		}
	}
	cout<<"drew.\n";
	return 0;
}
