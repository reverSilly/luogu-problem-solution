// 题目：P2049 魔术棋子
// 思路：DP，每个格子维护「走到这里可能的所有乘积 mod K 的余数集合」
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};

int m,n,k;
i64 a[mxn][mxn];
bitset<101> f[mxn][mxn];		// f[i][j]: 走到 (i,j) 后可能的所有余数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>k;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			cin>>a[i][j];
			a[i][j]=(a[i][j]%k+k)%k;		// 先取模，避免乘法溢出
		}
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			if(i==0&&j==0)
			{
				f[i][j].set((int)a[i][j]);
				continue;
			}
			// 从上方或左方走进来，乘上本格数字
			for(int r=0;r<k;++r)
			{
				if(i>0&&f[i-1][j].test(r)) f[i][j].set((int)(r*a[i][j]%k));
				if(j>0&&f[i][j-1].test(r)) f[i][j].set((int)(r*a[i][j]%k));
			}
		}
	bitset<101> res=f[m-1][n-1];
	int cnt=0;
	for(int r=0;r<k;++r) if(res.test(r)) ++cnt;
	cout<<cnt<<'\n';
	bool first=true;
	for(int r=0;r<k;++r)
		if(res.test(r))
		{
			if(!first) cout<<' ';
			first=false;
			cout<<r;
		}
	cout<<'\n';
	return 0;
}
