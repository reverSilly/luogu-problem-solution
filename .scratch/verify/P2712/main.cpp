// 题目：P2712 摄像头
// i 监视 x_j ⇒ 依赖边 i→j（j 要等 i 先被砸）；入度 0 可砸 ⇒ Kahn 拓扑剥皮
// 坑：自监视不算（"其他"摄像头）；重复监视只算一个前驱（bool 去重）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxv{505};

int n,x[mxn],in[mxn];
bool mon[mxn][mxv],e[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,m;i<n;++i)
	{
		cin>>x[i]>>m;
		for(int j=0,y;j<m;++j)
		{
			cin>>y;
			mon[i][y]=true;	// 重复列出幂等，天然去重
		}
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(i!=j&&mon[i][x[j]])	// i!=j：自监视不构成依赖
				e[i][j]=true,++in[j];
	queue<int>Q;
	for(int i=0;i<n;++i)
		if(!in[i])
			Q.push(i);
	int cnt=0;
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		++cnt;
		for(int v=0;v<n;++v)
			if(e[u][v]&&--in[v]==0)
				Q.push(v);
	}
	if(cnt==n)
		cout<<"YES\n";
	else
		cout<<n-cnt<<'\n';
	return 0;
}