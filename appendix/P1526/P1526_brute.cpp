//
// 题目：P1526 [NOI2003] 智破连环阵
// 暴力基准: 状压 BFS (状态 = (已用炸弹集合 mask, 当前待消灭武器下标 i)), 精确求最短路(仅小 n 可用)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20};

int m,n,k;
int wx[mxn],wy[mxn],bx[mxn],by[mxn];
int rch[mxn][mxn];

bool inRange(int b,int w)
{
	i64 dx=bx[b]-wx[w],dy=by[b]-wy[w];
	return dx*dx+dy*dy<=1LL*k*k;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>m>>n>>k))
		return 0;
	for(int i=0;i<m;++i)
		cin>>wx[i]>>wy[i];
	for(int i=0;i<n;++i)
		cin>>bx[i]>>by[i];

	for(int b=0;b<n;++b)
	{
		rch[b][m]=m-1;
		for(int i=m-1;i>=0;--i)
			rch[b][i]=inRange(b,i)?max(i,rch[b][i+1]):i-1;
	}

	// maxPos[mask] = 使用 mask 集合中的炸弹最多能推进到的待消灭武器下标
	int tot=1<<n;
	vector<int>maxPos(tot,-1);
	maxPos[0]=0;
	int ans=n+1;
	for(int mask=0;mask<tot;++mask)
	{
		int cur=maxPos[mask];
		if(cur<0)
			continue;
		int cnt=__builtin_popcount(mask);
		if(cur==m)
		{
			ans=min(ans,cnt);
			continue;
		}
		for(int b=0;b<n;++b)
			if(!(mask>>b&1)&&rch[b][cur]>=cur)
			{
				int nmask=mask|(1<<b);
				maxPos[nmask]=max(maxPos[nmask],rch[b][cur]+1);
			}
	}
	cout<<(ans>n?-1:ans)<<'\n';
	return 0;
}
