//
// 题目：P1900 [ICPC 1998 Mid-Central R] 自我数
// https://www.luogu.com.cn/problem/P1900
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2000005};

int n,k;
int sf[mxn];

int digsum(int x)
{
	int s=0;
	while(x){
		s+=x%10;
		x/=10;
	}
	return s;
}

bool isself(int x)
{
	int bound=x-1;
	int lo=x-200;
	if(lo<1){
		lo=1;
	}
	for(int g=lo;g<=bound;++g){
		if(g+digsum(g)==x){
			return false;
		}
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	int cnt=0;
	for(int i=1;i<=n;++i){
		if(isself(i)){
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
