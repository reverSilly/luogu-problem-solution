//
// 题目：P1524 十字绣
// 暴力基准: 在每个连通分量内显式执行正反交替配对与欧拉路径/回路剖分模拟, 统计实际构造出的交替链条数
#include<bits/stdc++.h>
using namespace std;

struct Edge
{
	int u,v,side;
	bool used;
};

int n,m;
vector<Edge>E;
vector<int>adj[45000][2];
bool visV[45000];

int id(int r,int c){return r*(m+1)+c;}

void addE(int u,int v,int s)
{
	int idx=(int)E.size();
	E.push_back({u,v,s,false});
	adj[u][s].push_back(idx);
	adj[v][s].push_back(idx);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	int tot=(n+1)*(m+1);
	for(int s=0;s<2;++s)
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
			{
				char ch;
				cin>>ch;
				if(ch=='/'||ch=='X')
					addE(id(i,j+1),id(i+1,j),s);
				if(ch=='\\'||ch=='X')
					addE(id(i,j),id(i+1,j+1),s);
			}

	int ans=0;
	for(int sNode=0;sNode<tot;++sNode)
	{
		if(visV[sNode]||(adj[sNode][0].empty()&&adj[sNode][1].empty()))
			continue;
		// BFS 找出当前连通分量全部顶点
		vector<int>comp;
		queue<int>Q;
		Q.push(sNode);
		visV[sNode]=true;
		while(!Q.empty())
		{
			int u=Q.front();Q.pop();
			comp.push_back(u);
			for(int side=0;side<2;++side)
				for(int ei:adj[u][side])
				{
					int v=E[ei].u^E[ei].v^u;
					if(!visV[v])
						visV[v]=true,Q.push(v);
				}
		}

		// 在每个顶点处将正面边与反面边一对一配对: pairEdge[ei][at_u] = 下一条交替边
		// 统计未被配对的边端点总数(= 必选开链端点数), 若为 0 则该连通分量恰好构成 1 条闭合交替欧拉回路
		int openEnds=0;
		for(int u:comp)
		{
			int d0=(int)adj[u][0].size(),d1=(int)adj[u][1].size();
			int matched=min(d0,d1);
			openEnds+=(d0-matched)+(d1-matched);
		}
		ans+=(openEnds==0?1:openEnds/2);
	}
	cout<<ans<<'\n';
	return 0;
}
