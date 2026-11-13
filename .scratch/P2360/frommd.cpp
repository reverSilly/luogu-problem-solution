// 题目：P2360 地下城主
// 三维无权图最短路: 六个方向 BFS, 每步 1 分钟
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{32};

int L,R,C;
char mp[mxn][mxn][mxn];
int dis[mxn*mxn*mxn],Q[mxn*mxn*mxn];
const int dx[6]={1,-1,0,0,0,0};
const int dy[6]={0,0,1,-1,0,0};
const int dz[6]={0,0,0,0,1,-1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>L>>R>>C)
	{
		if(L==0||R==0||C==0)
			break;
		int S=-1,E=-1;
		for(int i=0;i<L;++i)
			for(int j=0;j<R;++j)
				for(int k=0;k<C;++k)
				{
					cin>>mp[i][j][k];   // operator>> 会跳过层间空行
					if(mp[i][j][k]=='S')
						S=(i*R+j)*C+k;
					if(mp[i][j][k]=='E')
						E=(i*R+j)*C+k;
				}
		int tot=L*R*C;
		fill_n(dis,tot,-1);
		int hd=0,tl=0;
		Q[tl++]=S;
		dis[S]=0;
		while(hd<tl)
		{
			int u=Q[hd++];
			int i=u/(R*C),j=u/C%R,k=u%C;   // u/C 要对 R 取模
			for(int d=0;d<6;++d)
			{
				int ni=i+dx[d],nj=j+dy[d],nk=k+dz[d];
				if(ni<0||ni>=L||nj<0||nj>=R||nk<0||nk>=C)
					continue;
				if(mp[ni][nj][nk]=='#')
					continue;
				int v=(ni*R+nj)*C+nk;
				if(dis[v]!=-1)
					continue;
				dis[v]=dis[u]+1;
				Q[tl++]=v;
			}
		}
		if(dis[E]<0)
			cout<<"Trapped!"<<'\n';
		else
			cout<<"Escaped in "<<dis[E]<<" minute(s)."<<'\n';
	}
	return 0;
}
