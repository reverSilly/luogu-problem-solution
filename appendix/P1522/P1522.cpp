//
// 题目：P1522 [USACO2.4] 牛的旅行 Cow Tours
// https://www.luogu.com.cn/problem/P1522
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};           // N<=150 (+5 松弛)
constexpr double inf{1e18};

int n;
double px[mxn],py[mxn];
double dis[mxn][mxn];
double mxd[mxn];                  // mxd[i]=与 i 同连通块内的最远最短路距离(偏心率)
int comp[mxn];                    // comp[i]=i 所属连通块编号
double diam[mxn];                 // diam[c]=连通块 c 的原内部直径

double euc(int i,int j)
{
	return hypot(px[i]-px[j],py[i]-py[j]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];

	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			char ch;
			cin>>ch;
			if(i==j)
				dis[i][j]=0;
			else if(ch=='1')
				dis[i][j]=euc(i,j);
			else
				dis[i][j]=inf;
		}

	// Floyd-Warshall 全源最短路
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			if(dis[i][k]<inf/2)
				for(int j=0;j<n;++j)
					if(dis[k][j]<inf/2)
						dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);

	fill_n(comp,n,-1);
	int ccnt=0;
	for(int i=0;i<n;++i)
	{
		if(comp[i]==-1)
		{
			for(int j=0;j<n;++j)
				if(dis[i][j]<inf/2)
					comp[j]=ccnt;
			++ccnt;
		}
		for(int j=0;j<n;++j)
			if(dis[i][j]<inf/2)
				mxd[i]=max(mxd[i],dis[i][j]);
		diam[comp[i]]=max(diam[comp[i]],mxd[i]);
	}

	double ans=inf;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			if(comp[i]!=comp[j])
			{
				double nd=max({diam[comp[i]],diam[comp[j]],mxd[i]+euc(i,j)+mxd[j]});
				ans=min(ans,nd);
			}

	cout<<fixed<<setprecision(6)<<ans<<'\n';
	return 0;
}
