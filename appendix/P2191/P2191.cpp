// 题目：P2191 小Z的情书
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n;
char msk[mxn][mxn],tx[mxn][mxn];

// 顺时针旋转 90 度：new[i][j] = old[n-1-j][i]
void rot()
{
	static char tmp[mxn][mxn];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			tmp[i][j]=msk[n-1-j][i];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			msk[i][j]=tmp[i][j];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>msk[i];
	for(int i=0;i<n;++i)
		cin>>tx[i];
	string ans;
	// 只旋转透明纸，字母纸固定不动，共读 4 遍
	for(int t=0;t<4;++t)
	{
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(msk[i][j]=='O')
					ans+=tx[i][j];
		if(t<3)
			rot();
	}
	cout<<ans<<'\n';
	return 0;
}
