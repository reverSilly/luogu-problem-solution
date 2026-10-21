//
// 题目：P1808 单词分类
// https://www.luogu.com.cn/problem/P1808
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n;
int cnt[mxn][26];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int cls{0};
	string s;
	for(int i=0;i<n;++i)
	{
		cin>>s;
		int c[26]{};
		for(char ch:s)
			++c[ch-'A'];
		int found{-1};
		for(int j=0;j<cls;++j)
		{
			bool eq{true};
			for(int k=0;k<26;++k)
			{
				if(cnt[j][k]!=c[k])
				{
					eq=false;
					break;
				}
			}
			if(eq)
			{
				found=j;
				break;
			}
		}
		if(found<0)
		{
			for(int k=0;k<26;++k)
				cnt[cls][k]=c[k];
			++cls;
		}
	}
	cout<<cls<<'\n';
	return 0;
}
