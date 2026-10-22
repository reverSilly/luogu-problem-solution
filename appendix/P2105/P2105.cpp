// 题目：P2105 K皇后
#include<bits/stdc++.h>
using namespace std;

using i64=long long;

constexpr int mxn{20005};

int n,m,K;
int qx[mxn],qy[mxn];
bool rowHit[mxn],colHit[mxn];
int stamp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>K;
	for(int i=0;i<K;++i)
	{
		cin>>qx[i]>>qy[i];
		rowHit[qx[i]]=true;
		colHit[qy[i]]=true;
	}
	int colCnt=0;
	for(int j=1;j<=m;++j)
		if(colHit[j]) ++colCnt;

	i64 ans=0;
	for(int x=1;x<=n;++x)
	{
		if(rowHit[x]) continue;
		// 本行被对角线打到的列(去重, 且不含已被皇后占的列)
		int cnt=0;
		for(int i=0;i<K;++i)
		{
			int y1=x-(qx[i]-qy[i]);
			if(y1>=1 && y1<=m && !colHit[y1] && stamp[y1]!=x)
			{
				stamp[y1]=x;
				++cnt;
			}
			int y2=(qx[i]+qy[i])-x;
			if(y2>=1 && y2<=m && !colHit[y2] && stamp[y2]!=x)
			{
				stamp[y2]=x;
				++cnt;
			}
		}
		ans+=m-colCnt-cnt;
	}
	cout<<ans<<'\n';
	return 0;
}
