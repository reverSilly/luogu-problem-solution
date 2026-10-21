//
// 题目：P1886 【模板】单调队列 / 滑动窗口
// https://www.luogu.com.cn/problem/P1886
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int n,k;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=0;i<n;++i){
		cin>>a[i];
	}
	int w=n-k+1;
	for(int i=0;i<w;++i){
		int mn=a[i];
		int mx=a[i];
		for(int j=i;j<i+k;++j){
			if(a[j]<mn){
				mn=a[j];
			}
			if(a[j]>mx){
				mx=a[j];
			}
		}
		if(i){
			cout<<' ';
		}
		cout<<mn;
	}
	cout<<'\n';
	for(int i=0;i<w;++i){
		int mx=a[i];
		for(int j=i;j<i+k;++j){
			if(a[j]>mx){
				mx=a[j];
			}
		}
		if(i){
			cout<<' ';
		}
		cout<<mx;
	}
	cout<<'\n';
	return 0;
}
