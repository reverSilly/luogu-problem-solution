// 题目：P2083 找人
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr i64 inf{1LL<<60};

int n,m,v,tx,ty;
int succ[mxn];        // succ[u] = u 的唯一后继房间
int head[mxn];        // 反图邻接表头
int rnxt[mxn];
i64 dis[mxn];
int que[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>v>>tx>>ty;
	int tot=n*m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			int a,b;
			cin>>a>>b;
			succ[i*m+j]=(a-1)*m+(b-1);
		}
	for(int i=0;i<tot;++i)
		head[i]=-1;
	for(int u=0;u<tot;++u)
	{
		rnxt[u]=head[succ[u]];
		head[succ[u]]=u;
	}

	// 反向 BFS：原图每个房间只有一条出边，故反图上从同学出发到某房间的路径唯一
	int s=(tx-1)*m+(ty-1);
	fill_n(dis,tot,inf);
	int hh=0,tt=0;
	dis[s]=0;
	que[tt++]=s;
	while(hh<tt)
	{
		int u=que[hh++];
		for(int p=head[u];p!=-1;p=rnxt[p])
		{
			if(dis[p]!=inf)
				continue;
			dis[p]=dis[u]+abs(p/m-u/m)*v;   // 只按楼层差计体力，同层换房免费
			que[tt++]=p;
		}
	}

	i64 ans=inf;
	for(int j=0;j<m;++j)
		ans=min(ans,dis[j]);
	if(ans==inf)
		cout<<"impossible\n";
	else
		cout<<ans<<'\n';
	return 0;
}
