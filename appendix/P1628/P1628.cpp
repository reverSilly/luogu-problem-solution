//
// 题目：P1628 合并序列
// https://www.luogu.com.cn/problem/P1628
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n;
string s[mxn],t;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>s[i];
	cin>>t;
	sort(s,s+n);
	int len=t.size();
	for(int i=0;i<n;++i)
		if((int)s[i].size()>=len&&s[i].compare(0,len,t)==0)
			cout<<s[i]<<'\n';
	return 0;
}
