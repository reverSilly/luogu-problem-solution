// 题目：P2426 删数
// 思路：只从两端删 ⇒ 删掉的块是原数组的连续划分，总价值与顺序无关
//       区间 DP：f[l][r]=max_k( val(l,k)+f[k+1][r] )，val 单元素时为 x[l]
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,x[mxn],f[mxn][mxn];

int val(int l,int r){return l==r?x[l]:abs(x[l]-x[r])*(r-l+1);}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)cin>>x[i];
	// f[l][r]=max total value of partitioning [l,r] into end-removable segments
	for(int len=1;len<=n;++len)
		for(int l=1;l+len-1<=n;++l)
		{
			int r=l+len-1;
			f[l][r]=0;
			for(int k=l;k<=r;++k)
				f[l][r]=max(f[l][r],val(l,k)+(k<r?f[k+1][r]:0));
		}
	cout<<f[1][n]<<'\n';
	return 0;
}
