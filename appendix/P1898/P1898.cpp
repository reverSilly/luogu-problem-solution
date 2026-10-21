//
// 题目：P1898 缘分计算
// https://www.luogu.com.cn/problem/P1898
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s,cur;
	int st;
	cin>>s>>st;
	for(char c:s){
		cur+=to_string(st+(c-'A'));
	}
	while((int)cur.size()>2&&cur!="100"){
		string nxt;
		int len=(int)cur.size()-1;
		nxt.resize(len);
		for(int i=0;i<len;++i){
			int d=(cur[i]-'0'+cur[i+1]-'0')%10;
			nxt[i]=char('0'+d);
		}
		cur.swap(nxt);
	}
	int val=0;
	for(char c:cur){
		val=val*10+(c-'0');
	}
	cout<<val<<'\n';
	return 0;
}
