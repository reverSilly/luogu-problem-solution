//
// 题目：P1893 [USACO10OPEN] 山峰暸望 Mountain Watching B
// https://www.luogu.com.cn/problem/P1893
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n;
int h[mxn];

bool mount(int l,int r)
{
	int p=l;
	while(p<r&&h[p]<=h[p+1]){
		++p;
	}
	for(int i=p;i<r;++i){
		if(h[i]<h[i+1]){
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
	cin>>n;
	for(int i=0;i<n;++i){
		cin>>h[i];
	}
	int ans=0;
	for(int l=0;l<n;++l){
		for(int r=l;r<n;++r){
			if(!mount(l,r)){
				break;
			}
			int w=r-l+1;
			if(w>ans){
				ans=w;
			}
		}
	}
	cout<<ans<<'\n';
	return 0;
}
