//
// 题目：P1529 [USACO2.4] 回家 Bessie Come Home
// https://www.luogu.com.cn/problem/P1529
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int nn{52};             // a-z 26 个 + A-Y 25 个 + 谷仓 Z 1 个
constexpr int inf{1145141919};    // 114514 家族哨兵

int P;
int dis[nn][nn];                  // dis[u][v]=u 到 v 的最短路

int id(char c)                    // 字符 -> 0-based 标号
{
	if(c>='a'&&c<='z')
		return c-'a';              // 0..25 小写(无牛)
	if(c>='A'&&c<='Y')
		return c-'A'+26;           // 26..50 大写(每处一头牛)
	return 51;                     // 谷仓 Z
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>P;
	for(int i=0;i<nn;++i)
		for(int j=0;j<nn;++j)
			dis[i][j]=i==j?0:inf;
	for(int i=0;i<P;++i)
	{
		char a,b;int w;
		cin>>a>>b>>w;
		int u=id(a),v=id(b);
		if(w<dis[u][v])            // 重边取最小
		{
			dis[u][v]=w;
			dis[v][u]=w;
		}
	}
	for(int k=0;k<nn;++k)          // Floyd
		for(int i=0;i<nn;++i)
			for(int j=0;j<nn;++j)
				if(dis[i][k]<inf&&dis[k][j]<inf&&dis[i][k]+dis[k][j]<dis[i][j])
					dis[i][j]=dis[i][k]+dis[k][j];
	int best=inf,bi=-1;
	for(int i=26;i<=50;++i)        // A..Y 各有一头牛,取离谷仓最近者
		if(dis[i][51]<best)
		{
			best=dis[i][51];
			bi=i;
		}
	cout<<char('A'+bi-26)<<' '<<best<<'\n';
	return 0;
}
