#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int f[mxn];
string x,y;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y;
	int n=x.size(),m=y.size();
	for(int i=1;i<=n;++i)
	{
		int prv=0;	// f[i-1][j-1]
		char xi=x[i-1];
		for(int j=1;j<=m;++j)
		{
			int tmp=f[j];	// 旧 f[i-1][j]
			if(xi==y[j-1])
				f[j]=prv+1;
			else
				f[j]=max(f[j],f[j-1]);
			prv=tmp;
		}
	}
	cout<<f[m]<<'\n';
	return 0;
}