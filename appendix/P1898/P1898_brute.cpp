//
// 题目：P1898 缘分计算
// https://www.luogu.com.cn/problem/P1898
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxd{80};

int d[mxd],nd;

void pushnum(int x)
{
	int tmp[8],t=0;
	while(x){
		tmp[t++]=x%10;
		x/=10;
	}
	while(t){
		d[nd++]=tmp[--t];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	int st;
	cin>>s>>st;
	nd=0;
	for(char c:s){
		pushnum(st+(c-'A'));
	}
	while(nd>2){
		if(nd==3&&d[0]==1&&d[1]==0&&d[2]==0){
			break;
		}
		int e[mxd],ne=nd-1;
		for(int i=0;i<ne;++i){
			e[i]=(d[i]+d[i+1])%10;
		}
		for(int i=0;i<ne;++i){
			d[i]=e[i];
		}
		nd=ne;
	}
	int val=0;
	for(int i=0;i<nd;++i){
		val=val*10+d[i];
	}
	cout<<val<<'\n';
	return 0;
}
