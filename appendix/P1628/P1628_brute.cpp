//
// 题目：P1628 合并序列 （暴力对拍用）
// 逐个判断前缀，再用选择排序按字典序整理输出
#include<bits/stdc++.h>
using namespace std;

int n;
string s[3005],t;

bool pre(const string&w,const string&p)
{
	if(w.size()<p.size())
		return false;
	for(size_t i=0;i<p.size();++i)
		if(w[i]!=p[i])
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>s[i];
	cin>>t;
	string res[3005];
	int c=0;
	for(int i=0;i<n;++i)
		if(pre(s[i],t))
			res[c++]=s[i];
	for(int i=0;i<c;++i)
		for(int j=i+1;j<c;++j)
			if(res[j]<res[i])
				swap(res[i],res[j]);
	for(int i=0;i<c;++i)
		cout<<res[i]<<'\n';
	return 0;
}
