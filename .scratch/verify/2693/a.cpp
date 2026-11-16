// 题目：P2693 [USACO1.3] 号码锁 Combination Lock
// 思路：每把钥匙开一个容差盒 S(x)×S(y)×S(z)，|S(c)|=min(n,5)，至多 125 个组合。
//       答案 = 两盒之并，枚举 ≤250 个组合用 vis 去重，自动处理 n<5 与两盒相交。
//       圆环距离 ⇒ 偏移 dx∈[-2,2] 后 wrap 归一化；n≤5 时全空间开锁。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,f[3],g[3];
bool vis[mxn][mxn][mxn];

int wrap(int v){return (v%n+n)%n;}	// 归一化到 [0,n)，处理负偏移与绕圈

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<3;++i){cin>>f[i];--f[i];}
	for(int i=0;i<3;++i){cin>>g[i];--g[i];}
	int ans=0;
	for(int t=0;t<2;++t)
	{
		int*c=t?g:f;	// t=0 农夫，t=1 锁匠
		for(int dx=-2;dx<=2;++dx)
			for(int dy=-2;dy<=2;++dy)
				for(int dz=-2;dz<=2;++dz)
				{
					int a=wrap(c[0]+dx),b=wrap(c[1]+dy),d=wrap(c[2]+dz);
					if(!vis[a][b][d])
					{
						vis[a][b][d]=true;
						++ans;	// 每个不同组合只计一次
					}
				}
	}
	cout<<ans<<'\n';
	return 0;
}
