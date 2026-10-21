//
// P1588 朴素解：状态上界固定为 400000，不做 2y 剪枝
#include<bits/stdc++.h>
using namespace std;

constexpr int lim{400000};

int dis[lim+5];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int m;
	cin>>m;
	while(m--)
	{
		int s,t;
		cin>>s>>t;
		fill_n(dis,lim+1,-1);
		queue<int>Q;
		dis[s]=0;
		Q.push(s);
		while(!Q.empty())
		{
			int u=Q.front();
			Q.pop();
			if(u==t)
				break;
			int nx[3]{u+1,u-1,u*2};
			for(int k=0;k<3;++k)
			{
				int v=nx[k];
				if(v>=0&&v<=lim&&dis[v]==-1)
				{
					dis[v]=dis[u]+1;
					Q.push(v);
				}
			}
		}
		cout<<dis[t]<<'\n';
	}
	return 0;
}
