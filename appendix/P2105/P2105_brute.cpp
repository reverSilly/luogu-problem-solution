// 题目：P2105 K皇后 —— 暴力：标记攻击范围后逐格统计
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,m,K;
bool rowHit[mxn],colHit[mxn];
bool d1[mxn*2],d2[mxn*2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>K;
	for(int i=0;i<K;++i)
	{
		int x,y;cin>>x>>y;
		rowHit[x]=true;
		colHit[y]=true;
		d1[x+y]=true;
		d2[x-y+mxn]=true;
	}
	int ans=0;
	for(int x=1;x<=n;++x)
		for(int y=1;y<=m;++y)
			if(!rowHit[x] && !colHit[y] && !d1[x+y] && !d2[x-y+mxn])
				++ans;
	cout<<ans<<'\n';
	return 0;
}
