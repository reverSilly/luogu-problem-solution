//
// 题目：P1588 [USACO07OPEN] Catch That Cow S
// https://www.luogu.com.cn/problem/P1588
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int x,y;
int dis[mxn],Q[mxn];

int bfs(int s,int t)
{
	if(s>=t)
		return s-t;            // 位置已不低于牛, 只有 -1 有用
	fill_n(dis,2*t+2,-1);      // 只需清空 [0,2t], 更远处不可能最优
	int ql=0,qr=0;
	dis[s]=0;
	Q[qr++]=s;
	while(ql<qr)
	{
		int u=Q[ql++];
		if(u==t)
			return dis[u];
		int nx[3]{u+1,u-1,u*2};
		for(int k=0;k<3;++k)
		{
			int v=nx[k];
			if(v>=0&&v<=2*t&&dis[v]==-1)
			{
				dis[v]=dis[u]+1;
				Q[qr++]=v;
			}
		}
	}
	return dis[t];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int m;
	cin>>m;
	while(m--)
	{
		cin>>x>>y;
		cout<<bfs(x,y)<<'\n';
	}
	return 0;
}
