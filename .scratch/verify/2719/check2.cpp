#include<bits/stdc++.h>
using namespace std;
long double closed(int n)
{
	int m=n-1;
	long double p=1;
	for(int i=1;i<=m;++i)p=p*(m+i)/i/4;
	return 1-p;
}
int main()
{
	for(int n:{500,1000,1250})
	{
		vector<vector<long double>>f(n+1,vector<long double>(n+1,0));
		for(int s=2;s<=2*n;++s)
			for(int a=max(0,s-n);a<=min(n,s);++a)
			{
				int b=s-a;
				if(b<0||b>n)continue;
				if(s==2)f[a][b]=(a==2||b==2)?1.0L:0.0L;
				else if(a==0)f[a][b]=f[a][b-1];
				else if(b==0)f[a][b]=f[a-1][b];
				else f[a][b]=0.5L*(f[a-1][b]+f[a][b-1]);
			}
		long double c=closed(n),d=f[n][n];
		cout<<fixed<<setprecision(10)<<"n="<<n<<" closed="<<c<<" dp="<<d<<" diff="<<fabsl(c-d)<<"\n";
	}
	return 0;
}