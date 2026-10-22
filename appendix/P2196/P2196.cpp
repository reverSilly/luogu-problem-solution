// 题目：P2196 [NOIP 1996 提高组] 挖地雷
// 思路：编号只能往大的走，构成 DAG，逆序 DP
//       f[i] = 以 i 为起点能挖到的最大地雷数，f[i] = a[i] + max{f[j]}
//       nxt[i] 记录最优的下一个地窖，取编号最小者，保证路径字典序最小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n,a[mxn];
bool g[mxn][mxn];
int f[mxn],nxt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)cin>>a[i];
	// 上三角按行读入：第 i 行给出 i 与 i+1..n 的连通情况
	for(int i=1;i<n;++i)
		for(int j=i+1;j<=n;++j)
		{
			int x;cin>>x;
			g[i][j]=x;
		}
	for(int i=n;i>=1;--i)
	{
		f[i]=a[i];
		nxt[i]=0;
		for(int j=i+1;j<=n;++j)
			if(g[i][j]&&f[j]+a[i]>f[i])
			{
				f[i]=f[j]+a[i];
				nxt[i]=j;
			}
	}
	int st=1;
	for(int i=2;i<=n;++i)
		if(f[i]>f[st])st=i;
	bool fst=1;
	for(int u=st;u;u=nxt[u])
	{
		if(!fst)cout<<' ';
		fst=0;
		cout<<u;
	}
	cout<<'\n'<<f[st]<<'\n';
	return 0;
}
