//
// 题目：P1509 找啊找啊找GF
// https://www.luogu.com.cn/problem/P1509
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};   // n<=100 (+5 松弛)
constexpr int mxr{105};   // m,r<=100 (+5 松弛)

int n,m,r;
int rmb[mxn],rp[mxn],ti[mxn];
int cnt[mxr][mxr];        // cnt[j][k]=预算<=j 且点数<=k 时能泡到的最大人数
int tim[mxr][mxr];        // tim[j][k]=在最大人数前提下的最小总时间

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>rmb[i]>>rp[i]>>ti[i];
	cin>>m>>r;

	for(int i=0;i<n;++i)
		for(int j=m;j>=rmb[i];--j)
			for(int k=r;k>=rp[i];--k)
			{
				int c2=cnt[j-rmb[i]][k-rp[i]]+1;
				int t2=tim[j-rmb[i]][k-rp[i]]+ti[i];
				if(c2>cnt[j][k]||(c2==cnt[j][k]&&t2<tim[j][k]))
				{
					cnt[j][k]=c2;
					tim[j][k]=t2;
				}
			}

	cout<<tim[m][r]<<'\n';
	return 0;
}
