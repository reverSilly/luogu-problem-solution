// 题目：P2213 [USACO14MAR] The Lazy Cow S
// 曼哈顿菱形旋转 45 度变成轴平行正方形，再用二维前缀和 O(1) 查询
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{405},mxx{810};

int n,k;
i64 s[mxx][mxx];

// 1-based 的矩形和，闭区间 [u1,u2] x [v1,v2]
i64 rect(int u1,int v1,int u2,int v2)
{
	return s[u2][v2]-s[u1-1][v2]-s[u2][v1-1]+s[u1-1][v1-1];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int r=0,g;r<n;++r)
		for(int c=0;c<n;++c)
		{
			cin>>g;
			s[r+c+1][r-c+n]=g;   // (r,c) -> (u,v)=(r+c,r-c)，下标各 +1 / +n
		}
	int mx=2*n-1;
	for(int i=1;i<=mx;++i)
		for(int j=1;j<=mx;++j)
			s[i][j]+=s[i-1][j]+s[i][j-1]-s[i-1][j-1];
	i64 ans=0;
	for(int r=0;r<n;++r)
		for(int c=0;c<n;++c)
		{
			int u=r+c+1,v=r-c+n;
			int u1=max(1,u-k),u2=min(mx,u+k);
			int v1=max(1,v-k),v2=min(mx,v+k);
			ans=max(ans,rect(u1,v1,u2,v2));
		}
	cout<<ans<<'\n';
	return 0;
}
