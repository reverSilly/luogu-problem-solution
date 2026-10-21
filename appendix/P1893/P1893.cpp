//
// 题目：P1893 [USACO10OPEN] 山峰暸望 Mountain Watching B
// https://www.luogu.com.cn/problem/P1893
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n;
int h[mxn];
int lf[mxn],rg[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i){
		cin>>h[i];
	}
	lf[0]=0;
	for(int i=1;i<n;++i){
		if(h[i-1]<=h[i]){
			lf[i]=lf[i-1];
		}else{
			lf[i]=i;
		}
	}
	rg[n-1]=n-1;
	for(int i=n-2;i>=0;--i){
		if(h[i]>=h[i+1]){
			rg[i]=rg[i+1];
		}else{
			rg[i]=i;
		}
	}
	int ans=0;
	for(int i=0;i<n;++i){
		int w=rg[i]-lf[i]+1;
		if(w>ans){
			ans=w;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
