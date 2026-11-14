// P2683 轻量验证（进程内，跑得快）：
//   把 a.cpp 的动态 Floyd 逻辑原样搬过来，与"每次加边后从邻接矩阵从头跑 Floyd"的暴力
//   逐格比对整张矩阵 + 逐个询问答案。2000 组随机小数据。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10},inf{1145141919};

int n,m,d[mxn][mxn],g[mxn][mxn],bf[mxn][mxn];

void floyd()	// 暴力：从邻接矩阵从头算
{
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			bf[i][j]=(i==j?0:g[i][j]);
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if((i64)bf[i][k]+bf[k][j]<bf[i][j])
					bf[i][j]=bf[i][k]+bf[k][j];
}

int main()
{
	mt19937 rn(20260714);
	int cases=2000,bad=0,ins=0,qry=0,skipped=0;
	for(int tc=0;tc<cases;++tc)
	{
		n=1+rn()%8;
		m=1+rn()%60;
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				d[i][j]=g[i][j]=(i==j?0:inf);
		for(int q=0;q<m;++q)
		{
			int op,u,v;
			if(n==1)
				op=0,u=v=0;	// 只有一个点：无法构造 u!=v，退化为自查询
			else
			{
				op=rn()%2,u=rn()%n,v=rn()%n;
				while(v==u)
					v=rn()%n;
			}
			if(op)
			{
				int e=1+rn()%20;
				g[u][v]=g[v][u]=min(g[u][v],e);
				++ins;
				// ===== 与 a.cpp 完全一致的逻辑 =====
				if(e>=d[u][v])
				{
					++skipped;
					floyd();
					for(int i=0;i<n;++i)
						for(int j=0;j<n;++j)
							if(d[i][j]!=bf[i][j])
								++bad;
					continue;
				}
				d[u][v]=d[v][u]=e;
				for(int i=0;i<n;++i)
					for(int j=0;j<n;++j)
					{
						i64 t=min((i64)d[i][u]+e+d[v][j],(i64)d[i][v]+e+d[u][j]);
						if(t<d[i][j])
							d[i][j]=(int)t;
					}
				// ===== 逐格比对整张矩阵 =====
				floyd();
				for(int i=0;i<n;++i)
					for(int j=0;j<n;++j)
						if(d[i][j]!=bf[i][j])
						{
							++bad;
							if(bad<5)
								printf("MISMATCH tc=%d q=%d (%d,%d) dyn=%d brute=%d\n",tc,q,i,j,d[i][j],bf[i][j]);
						}
			}
			else
			{
				++qry;
				floyd();
				int a=(d[u][v]>=inf?-1:d[u][v]),b=(bf[u][v]>=inf?-1:bf[u][v]);
				if(a!=b)
				{
					++bad;
					if(bad<5)
						printf("QUERY MISMATCH tc=%d q=%d %d->%d dyn=%d brute=%d\n",tc,q,u,v,a,b);
				}
			}
		}
	}
	printf("cases=%d insertions=%d (skipped=%d) queries=%d mismatch=%d\n",cases,ins,skipped,qry,bad);
	return 0;
}