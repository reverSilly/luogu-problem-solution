// 题目：P2504 [HAOI2006] 聪明的猴子
// 思路：猴子能遍历所有树 ⟺ 只保留长度≤跳跃力的边时图连通
//       ⟺ 最小生成树的最大边（瓶颈）≤跳跃力（MST 即最小瓶颈生成树）
//       全程用距离平方比较，避开浮点：d≤jump ⟺ d²≤jump²
//       Prim O(N²) 求 MST 瓶颈，再数 jump²≥瓶颈² 的猴子
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxm{505},inf{1145141919};

int m,n,jmp[mxm],x[mxn],y[mxn],dis[mxn];
bool vis[mxn];

int d2(int u,int v)
{
	int dx=x[u]-x[v],dy=y[u]-y[v];
	return dx*dx+dy*dy;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m;
	for(int i=0;i<m;++i)
		cin>>jmp[i];
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	fill_n(dis,n,inf);
	dis[0]=0;
	int bst=0;	// MST 最大边的平方（瓶颈），平方域下比较
	for(int t=0;t<n;++t)
	{
		int u=-1;
		for(int v=0;v<n;++v)
			if(!vis[v]&&(u==-1||dis[v]<dis[u]))
				u=v;
		vis[u]=true;
		bst=max(bst,dis[u]);
		for(int v=0;v<n;++v)
			if(!vis[v])
				dis[v]=min(dis[v],d2(u,v));
	}
	int ans=0;
	for(int i=0;i<m;++i)
		if(jmp[i]*jmp[i]>=bst)
			++ans;
	cout<<ans<<'\n';
	return 0;
}
