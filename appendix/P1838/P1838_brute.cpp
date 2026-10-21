//
// 题目：P1838 三子棋I 暴力（位掩码枚举八条胜利线）
// https://www.luogu.com.cn/problem/P1838
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int winm[8]{7,56,448,73,146,292,273,84};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	int x=0,o=0;
	for(int i=0;i<(int)s.size();++i)
	{
		int bit=1<<(s[i]-'1');
		if(i%2==0)
			x|=bit;
		else
			o|=bit;
		int cur=i%2==0?x:o;
		bool ok=false;
		for(int k=0;k<8;++k)
			if((cur&winm[k])==winm[k])
				ok=true;
		if(ok)
		{
			if(i%2==0)
				cout<<"xiaoa wins.\n";
			else
				cout<<"uim wins.\n";
			return 0;
		}
	}
	cout<<"drew.\n";
	return 0;
}
