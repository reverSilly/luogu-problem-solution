// 在立方体表面建网格图, Dijkstra 近似测地线, 用于验证解析解 (只会偏大)
#include<bits/stdc++.h>
using namespace std;

constexpr int N{200}; // 每面细分

map<array<long long,3>,int>id;
vector<array<double,3>>pt;
vector<vector<pair<int,double>>>g;

int getid(double x,double y,double z)
{
	array<long long,3>k={llround(x*1e6),llround(y*1e6),llround(z*1e6)};
	auto it=id.find(k);
	if(it!=id.end())
		return it->second;
	int u=pt.size();
	id[k]=u;
	pt.push_back({x,y,z});
	g.push_back({});
	return u;
}

void addedge(int u,int v)
{
	double w=sqrt(pow(pt[u][0]-pt[v][0],2)+pow(pt[u][1]-pt[v][1],2)+pow(pt[u][2]-pt[v][2],2));
	g[u].push_back({v,w});
	g[v].push_back({u,w});
}

int main()
{
	double x1,y1,x2,y2;
	cin>>x1>>y1>>x2>>y2;
	// 6 faces: z=1, z=0, x=0.5, x=-0.5, y=0.5, y=-0.5
	vector<array<double,3>>base={{0,0,1},{0,0,0},{0.5,0,0.5},{-0.5,0,0.5},{0,0.5,0.5},{0,-0.5,0.5}};
	// 每面两个切向单位向量
	vector<array<double,3>>t1={{1,0,0},{1,0,0},{0,1,0},{0,1,0},{1,0,0},{1,0,0}};
	vector<array<double,3>>t2={{0,1,0},{0,1,0},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
	for(int f=0;f<6;++f)
	{
		vector<vector<int>>gid(N+1,vector<int>(N+1));
		for(int i=0;i<=N;++i)
			for(int j=0;j<=N;++j)
			{
				double a=-0.5+1.0*i/N,b=-0.5+1.0*j/N;
				double x=base[f][0]+a*t1[f][0]+b*t2[f][0];
				double y=base[f][1]+a*t1[f][1]+b*t2[f][1];
				double z=base[f][2]+a*t1[f][2]+b*t2[f][2];
				gid[i][j]=getid(x,y,z);
			}
		for(int i=0;i<=N;++i)
			for(int j=0;j<=N;++j)
				for(int di=-1;di<=1;++di)
					for(int dj=-1;dj<=1;++dj)
					{
						if(!di&&!dj)
							continue;
						int ni=i+di,nj=j+dj;
						if(ni<0||ni>N||nj<0||nj>N)
							continue;
						if(di<0||(di==0&&dj<0))
							continue;
						addedge(gid[i][j],gid[ni][nj]);
					}
	}
	int s=getid(x1,y1,1),t=getid(x2,y2,0);
	int n=pt.size();
	vector<double>dis(n,1e18);
	priority_queue<pair<double,int>,vector<pair<double,int>>,greater<>>Q;
	dis[s]=0;
	Q.push({0,s});
	while(!Q.empty())
	{
		auto[d,u]=Q.top();
		Q.pop();
		if(d>dis[u]+1e-12)
			continue;
		for(auto[v,w]:g[u])
			if(dis[v]>d+w)
			{
				dis[v]=d+w;
				Q.push({dis[v],v});
			}
	}
	printf("%.6f\n",dis[t]);
	return 0;
}
