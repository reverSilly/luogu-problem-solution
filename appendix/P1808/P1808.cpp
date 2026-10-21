//
// 题目：P1808 单词分类
// https://www.luogu.com.cn/problem/P1808
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	set<string> bag;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		sort(s.begin(),s.end());
		bag.insert(s);
	}
	cout<<bag.size()<<'\n';
	return 0;
}
