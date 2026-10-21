//
// 题目：P1840 Color the Axis 暴力（逐点染色）
// https://www.luogu.com.cn/problem/P1840
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{4005};

int n,m,black;
char blk[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=1;i<=n;++i)
		blk[i]=1;
	black=n;
	for(int i=0;i<m;++i)
	{
		int l,r;
		cin>>l>>r;
		for(int x=l;x<=r;++x)
		{
			if(blk[x])
			{
				blk[x]=0;
				--black;
			}
		}
		cout<<black<<'\n';
	}
	return 0;
}
