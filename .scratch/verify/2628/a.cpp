// 题目：P2628 冒险岛
// 模拟棋子行走：连续段长 k>=3 才触发奖励/惩罚，每回合至多一次
// 预处理 run[i]（同字符连续段长）把每回合降到 O(1)，总 O(L+n)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxl{1000005};

int L,n,run[mxl];
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	getline(cin,s);	// 路可能含空格，必须读整行
	if(!s.empty()&&s.back()=='\r')s.pop_back();	// 防 CRLF
	L=s.size();
	run[L-1]=1;
	for(int i=L-2;i>=0;--i)	// run[i]：从 i 起同字符连续段长度
		run[i]=(s[i]==s[i+1])?run[i+1]+1:1;
	cin>>n;
	int pos=-1;	// 起点：所有符号之前的第 0 格
	for(int i=0,d;i<n;++i)
	{
		cin>>d;
		pos+=d;
		if(pos>=L)pos=L-1;	// 超出右边界停在最后一个符号
		if(s[pos]=='>')
		{
			int k=run[pos];
			if(k>=3)pos=min(pos+k,L-1);	// 前进到第一个非 '>' 或右边界
		}
		else if(s[pos]=='*')
		{
			int k=run[pos];
			if(k>=3)pos=max(pos-k,0);	// 后退 k，超出左边界停在第一个符号
		}
	}
	cout<<pos+1<<' '<<L-1-pos<<'\n';
	return 0;
}
