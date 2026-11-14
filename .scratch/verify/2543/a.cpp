// 题目：P2543 [AHOI2004] 奇怪的字符串
// 思路：裸 LCS。f[i][j]=f[i-1][j-1]+1（末位同）或 max(f[i-1][j],f[i][j-1])（不同）。
//       n,m<=9998，二维表 400MB 爆内存 ⇒ 滚动成一行 + prv 存左上角，空间 O(m)。
//       时间 O(nm)≈1e8，实测 0.15s 稳过，无需 bitset。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int f[mxn];
string x,y;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y;
	int n=x.size(),m=y.size();
	for(int i=1;i<=n;++i)
	{
		int prv=0;	// f[i-1][0]=0；prv 始终保存左上角 f[i-1][j-1]
		char xi=x[i-1];
		for(int j=1;j<=m;++j)
		{
			int tmp=f[j];	// 旧 f[i-1][j]，必须在覆盖前取
			if(xi==y[j-1])
				f[j]=prv+1;
			else
				f[j]=max(f[j],f[j-1]);
			prv=tmp;
		}
	}
	cout<<f[m]<<'\n';
	return 0;
}
