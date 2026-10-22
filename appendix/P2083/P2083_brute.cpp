#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr i64 inf{1LL<<60};

int n,m,v,tx,ty;
int succ[mxn];        // succ[u] = 下一个房间
bool vis[mxn];
i64 dis[mxn];

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
	int s=(tx-1)*m+(ty-1);

	// 暴力：从第一层每个房间出发顺着指引走，靠 vis 判环
	for(int j=0;j<m;++j)
	{
		fill_n(vis,tot,false);
		i64 sum=0;
		int u=j;
		while(!vis[u])
		{
			if(u==s)
			{
				dis[j]=sum;
				break;
			}
			vis[u]=true;
			int w=succ[u];
			sum+=abs(w/m-u/m)*v;   // 只按楼层差计体力，同层换房免费
			u=w;
		}
		if(u!=s)
			dis[j]=inf;
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
