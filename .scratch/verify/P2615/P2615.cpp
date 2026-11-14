// 题目：P2615 [NOIP 2015 提高组] 神奇的幻方
// 思路：纯模拟。记录上一个数 K-1 的位置 (r,c)，按四条规则分类讨论填 K。
//       规则 3（右上角）必须先于规则 1/2 判断；规则 4 用 a[r-1][c+1]==0 判空。
//       0-based：第一行 r=0，中间列 c=n/2（n 为奇数），最后一行/列 n-1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{45};

int n,a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int r=0,c=n/2;
	a[r][c]=1;
	for(int k=2;k<=n*n;++k)
	{
		int nr,nc;
		if(r==0&&c!=n-1)
			nr=n-1,nc=c+1;			// 规则 1：第一行非末列 → 最后一行右一列
		else if(c==n-1&&r!=0)
			nr=r-1,nc=0;			// 规则 2：末列非首行 → 第一列上一行
		else if(r==0&&c==n-1)
			nr=r+1,nc=c;			// 规则 3：右上角 → 正下方
		else if(a[r-1][c+1]==0)
			nr=r-1,nc=c+1;			// 规则 4：右上方未填 → 右上方
		else
			nr=r+1,nc=c;			// 规则 4：右上方已填 → 正下方
		r=nr;c=nc;
		a[r][c]=k;
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cout<<a[i][j]<<(j==n-1?'\n':' ');
	return 0;
}