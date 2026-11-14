// 题目：P2758 编辑距离
// f[i][j]：把 A 的前 i 个字符变成 B 的前 j 个字符的最少操作数
// 末步分类：删 a_i / 插 b_j / 改(或保留) a_i->b_j
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n,m,f[mxn][mxn];
string a,b;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	n=a.size();m=b.size();
	for(int i=0;i<=n;++i)f[i][0]=i;	// 删光 A 的前 i 个
	for(int j=0;j<=m;++j)f[0][j]=j;	// 插入 B 的前 j 个
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
		{
			f[i][j]=min(f[i-1][j],f[i][j-1])+1;	// 删 / 插
			f[i][j]=min(f[i][j],f[i-1][j-1]+(a[i-1]!=b[j-1]));	// 改 / 保留
		}
	cout<<f[n][m]<<'\n';
	return 0;
}