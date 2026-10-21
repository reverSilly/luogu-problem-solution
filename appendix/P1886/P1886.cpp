//
// 题目：P1886 【模板】单调队列 / 滑动窗口
// https://www.luogu.com.cn/problem/P1886
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int n,k,w;
int a[mxn];
int q[mxn];
int amin[mxn],amax[mxn];

void slide(int *out,int mode)
{
	int hd=0,tl=0,cnt=0;
	for(int i=0;i<n;++i){
		if(mode==0){
			while(hd<tl&&a[q[tl-1]]>=a[i]){
				--tl;
			}
		}else{
			while(hd<tl&&a[q[tl-1]]<=a[i]){
				--tl;
			}
		}
		q[tl++]=i;
		if(q[hd]<=i-k){
			++hd;
		}
		if(i>=k-1){
			out[cnt++]=a[q[hd]];
		}
	}
}

void dump(int *b)
{
	for(int i=0;i<w;++i){
		if(i){
			cout<<' ';
		}
		cout<<b[i];
	}
	cout<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=0;i<n;++i){
		cin>>a[i];
	}
	w=n-k+1;
	slide(amin,0);
	slide(amax,1);
	dump(amin);
	dump(amax);
	return 0;
}
