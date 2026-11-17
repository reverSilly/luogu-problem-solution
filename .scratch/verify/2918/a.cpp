// 题目：P2918 [USACO08NOV] Buying Hay S
// f[j]=买到至少 j 磅的最小花费；买一包 i 后剩余需求 max(0,j-p[i])，
// 超重被截断吸收 ⇒ 状态空间恰好 [0,H]，无需"恰好 j + 后缀 min"的上界讨论。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxh{50005},inf{1145141919};

int n,h,p[mxn],c[mxn],f[mxh];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>h;
	for(int i=0;i<n;++i)
		cin>>p[i]>>c[i];
	fill_n(f,h+1,inf);	// 默认 0 会被 min 当成"免费达标"，必须显式置 inf
	f[0]=0;	// 需求 0 磅：什么都不买
	for(int j=1;j<=h;++j)
		for(int i=0;i<n;++i)
			f[j]=min(f[j],f[max(0,j-p[i])]+c[i]);	// 下标 <j，已算完
	cout<<f[h]<<'\n';
	return 0;
}
