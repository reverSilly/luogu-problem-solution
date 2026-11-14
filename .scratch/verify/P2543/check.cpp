#include<bits/stdc++.h>
using namespace std;
int roll(const string&x,const string&y)
{
	int n=x.size(),m=y.size();
	vector<int>f(m+1,0);
	for(int i=1;i<=n;++i)
	{
		int prv=0;char xi=x[i-1];
		for(int j=1;j<=m;++j)
		{
			int tmp=f[j];
			if(xi==y[j-1])f[j]=prv+1;
			else f[j]=max(f[j],f[j-1]);
			prv=tmp;
		}
	}
	return f[m];
}
int full(const string&x,const string&y)
{
	int n=x.size(),m=y.size();
	vector<vector<int>>g(n+1,vector<int>(m+1,0));
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
			g[i][j]=(x[i-1]==y[j-1])?g[i-1][j-1]+1:max(g[i-1][j],g[i][j-1]);
	return g[n][m];
}
int main()
{
	mt19937 rn(999);
	int bad=0;
	for(int t=0;t<3000;++t)
	{
		int n=rn()%40,m=rn()%40;
		string x,y;
		for(int i=0;i<n;++i)x+=char('0'+(rn()&1));
		for(int i=0;i<m;++i)y+=char('0'+(rn()&1));
		int a=roll(x,y),b=full(x,y);
		if(a!=b){++bad;if(bad<=5)cerr<<"MISMATCH x="<<x<<" y="<<y<<" roll="<<a<<" full="<<b<<"\n";}
	}
	cerr<<"done mismatches="<<bad<<"\n";
	return 0;
}