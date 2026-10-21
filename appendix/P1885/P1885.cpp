//
// 题目：P1885 [USACO12FEB] Moo B
// https://www.luogu.com.cn/problem/P1885
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{65};

i64 len[mxn];

char ask(int k,i64 n)
{
	if(k==0){
		if(n==1){
			return 'm';
		}
		return 'o';
	}
	i64 left=len[k-1];
	i64 mid=k+3;
	if(n<=left){
		return ask(k-1,n);
	}
	if(n<=left+mid){
		if(n==left+1){
			return 'm';
		}
		return 'o';
	}
	return ask(k-1,n-left-mid);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	cin>>n;
	len[0]=3;
	int k=0;
	while(len[k]<n){
		++k;
		len[k]=len[k-1]*2+(k+3);
	}
	cout<<ask(k,n)<<'\n';
	return 0;
}
