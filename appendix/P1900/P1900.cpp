//
// 题目：P1900 [ICPC 1998 Mid-Central R] 自我数
// https://www.luogu.com.cn/problem/P1900
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10000005};

int n,k;
bool bad[mxn];
int sf[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=1;i<=n;++i){
		int s=i,x=i;
		while(s){
			x+=s%10;
			s/=10;
		}
		if(x<=n){
			bad[x]=true;
		}
	}
	int cnt=0;
	for(int i=1;i<=n;++i){
		if(!bad[i]){
			sf[++cnt]=i;
		}
	}
	cout<<cnt<<'\n';
	for(int i=0;i<k;++i){
		int s;
		cin>>s;
		if(i){
			cout<<' ';
		}
		cout<<sf[s];
	}
	cout<<'\n';
	return 0;
}
