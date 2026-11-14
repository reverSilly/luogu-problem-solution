// P2683 轻量验证：随机小数据 + 每次操作后从头跑一遍 Floyd 的暴力
// 用法：gen.exe <seed> → 生成 test.in / exp.out
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10},inf{1145141919};

int n,m,g[mxn][mxn],d[mxn][mxn];

void floyd()	// 从邻接矩阵从头算全源最短路
{
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			d[i][j]=(i==j?0:g[i][j]);
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if((long long)d[i][k]+d[k][j]<d[i][j])
					d[i][j]=d[i][k]+d[k][j];
}

int main(int argc,char**argv)
{
	mt19937 rn(argc>1?atoi(argv[1]):12345);
	n=1+rn()%8;
	m=1+rn()%60;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			g[i][j]=(i==j?0:inf);
	FILE*fi=fopen("test.in","w");
	FILE*fo=fopen("exp.out","w");
	fprintf(fi,"%d %d\n",n,m);
	for(int q=0;q<m;++q)
	{
		int op,u,v;
		if(n==1)
			op=0,u=v=0;	// 只有一个点时无法构造 u!=v，退化为自查询
		else
		{
			op=rn()%2;
			u=rn()%n,v=rn()%n;
			while(v==u)
				v=rn()%n;
		}
		if(op)
		{
			int e=1+rn()%20;
			fprintf(fi,"1 %d %d %d\n",u+1,v+1,e);
			g[u][v]=g[v][u]=min(g[u][v],e);
		}
		else
		{
			fprintf(fi,"0 %d %d\n",u+1,v+1);
			floyd();
			fprintf(fo,"%d\n",d[u][v]>=inf?-1:d[u][v]);
		}
	}
	fclose(fi);
	fclose(fo);
	return 0;
}