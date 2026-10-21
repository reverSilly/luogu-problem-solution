//
// 题目：P1609 最小回文数
// https://www.luogu.com.cn/problem/P1609
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

string s;

string mk(const string&h,bool odd)         // 由左半段 h 造出回文
{
	string r=h;
	if(odd)
		r.pop_back();                      // 奇数长度时中间那位被共用
	reverse(r.begin(),r.end());
	return h+r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int L=s.size();
	bool all9=true;
	for(char ch:s)
		if(ch!='9')
			all9=false;
	if(all9)
	{
		cout<<'1'<<string(L-1,'0')<<'1'<<'\n';
		return 0;
	}
	int hl=(L+1)/2;                        // 左半段长度
	bool odd=(L&1);
	string h=s.substr(0,hl);
	string t=mk(h,odd);
	if(t>s)                                // 等长数字串的字典序就是数值序
	{
		cout<<t<<'\n';
		return 0;
	}
	int i=hl-1;
	while(i>=0&&h[i]=='9')
	{
		h[i]='0';
		--i;
	}
	if(i<0)                                // 左半段全是 9, 位数要涨(上面已排除, 兜底)
	{
		cout<<'1'<<string(L-1,'0')<<'1'<<'\n';
		return 0;
	}
	++h[i];
	cout<<mk(h,odd)<<'\n';
	return 0;
}
