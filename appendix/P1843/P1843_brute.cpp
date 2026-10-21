//
// 题目：P1843 奶牛晒衣服 暴力（每秒烘干最湿的一件）
// https://www.luogu.com.cn/problem/P1843
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};

int n,a,b;
i64 w[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>a>>b;
	for(int i=0;i<n;++i)
		cin>>w[i];
	i64 t=0;
	while(true)
	{
		bool wet=false;
		for(int i=0;i<n;++i)
			if(w[i]>0)
				wet=true;
		if(!wet)
			break;
		++t;
		for(int i=0;i<n;++i)
			w[i]-=a;
		int j=0;
		for(int i=1;i<n;++i)
			if(w[i]>w[j])
				j=i;
		w[j]-=b;
	}
	cout<<t<<'\n';
	return 0;
}
