// 题目：P2513 [HAOI2009] 逆序对数列
// 思路：f[i][j]=长度 i 恰好 j 个逆序对的排列数
//       把最大元素 i 插入长度 i-1 的排列，放到"后面恰有 t 个元素"的位置新增 t 个逆序对
//       f[i][j]=Σ_{t=0..min(j,i-1)} f[i-1][j-t]，是上一行的滑动窗口和
//       前缀和差分：f[i][j]=f[i][j-1]+f[i-1][j]-(j>=i?f[i-1][j-i]:0)，O(nk)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},p{10000};

int n,k,f[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	f[0][0]=1;	// 空排列 0 个逆序对
	for(int i=1;i<=n;++i)
		for(int j=0;j<=k;++j)
		{
			f[i][j]=f[i-1][j];
			if(j>0)
				f[i][j]+=f[i][j-1];
			if(j>=i)
				f[i][j]-=f[i-1][j-i];
			f[i][j]=(f[i][j]%p+p)%p;	// 减法可能出负数
		}
	cout<<f[n][k]<<'\n';
	return 0;
}
