#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxp{505};

int n,x[mxn],deg[mxn];
bool mon[mxn][mxp],blk[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		int m;
		cin>>x[i]>>m;
		for(int j=0,y;j<m;++j)
		{
			cin>>y;
			mon[i][y]=true;
		}
	}
	for(int j=0;j<n;++j)
		for(int i=0;i<n;++i)
			if(i!=j&&mon[j][x[i]])
				blk[j][i]=true,++deg[i];
	queue<int>Q;
	for(int i=0;i<n;++i)
		if(!deg[i])
			Q.push(i);
	int cnt=0;
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();++cnt;
		for(int v=0;v<n;++v)
			if(blk[u][v]&&--deg[v]==0)
				Q.push(v);
	}
	if(cnt==n)
		cout<<"YES\n";
	else
		cout<<n-cnt<<'\n';
	return 0;
}