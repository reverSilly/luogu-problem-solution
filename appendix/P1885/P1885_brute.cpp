//
// 题目：P1885 [USACO12FEB] Moo B
// https://www.luogu.com.cn/problem/P1885
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	cin>>n;
	string s="moo";
	int k=0;
	while((i64)s.size()<n){
		++k;
		string mid="m";
		mid.append(k+2,'o');
		string t=s;
		s+=mid;
		s+=t;
	}
	cout<<s[(size_t)n-1]<<'\n';
	return 0;
}
