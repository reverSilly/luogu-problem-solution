//
// 题目：P1509 找啊找啊找GF (暴力基准)
// https://www.luogu.com.cn/problem/P1509
// 仅用于小规模对拍: 枚举全部子集, 先取人数最大再取时间最小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n,m,r;
int rmb[mxn],rp[mxn],ti[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>rmb[i]>>rp[i]>>ti[i];
	cin>>m>>r;

	int bc=0,bt=0;            // 最多人数 及 该人数下的最小总时间
	for(int mask=0;mask<(1<<n);++mask)
	{
		int sr=0,sp=0,st=0,c=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1)
			{
				sr+=rmb[i];sp+=rp[i];st+=ti[i];++c;
			}
		if(sr<=m&&sp<=r&&(c>bc||(c==bc&&st<bt)))
		{
			bc=c;
			bt=st;
		}
	}
	cout<<bt<<'\n';
	return 0;
}
