// 题目：P2117 小Z的矩阵 —— 暴力: 真实维护矩阵, 每次询问直接按定义算 G
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{12};

int n,q;
int a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	string out;
	for(int k=0;k<q;++k)
	{
		int op;cin>>op;
		if(op==3)
		{
			int g=0;
			for(int i=0;i<n;++i)
				for(int j=0;j<n;++j)
					g^=a[i][j]&a[j][i];
			out+=char('0'+g);
		}
		else
		{
			int x;cin>>x;--x;
			if(op==1)
				for(int j=0;j<n;++j) a[x][j]^=1;
			else
				for(int i=0;i<n;++i) a[i][x]^=1;
		}
	}
	cout<<out<<'\n';
	return 0;
}
