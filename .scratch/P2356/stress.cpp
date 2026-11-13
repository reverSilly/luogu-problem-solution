#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
int a[mxn][mxn],rs[mxn],cs[mxn];

string fast(int n,const vector<vector<int>>&g)
{
	memset(rs,0,sizeof(rs));
	memset(cs,0,sizeof(cs));
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			a[i][j]=g[i][j];
			rs[i]+=a[i][j];
			cs[j]+=a[i][j];
		}
	int ans=-1;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(a[i][j]==0)
				ans=max(ans,rs[i]+cs[j]);
	if(ans<0)
		return string("Bad Game!");
	return to_string(ans);
}

string brute(int n,const vector<vector<int>>&a)
{
	int ans=-1;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(a[i][j]==0)
			{
				int s=0;
				for(int c=0;c<n;++c)
					s+=a[i][c];
				for(int r=0;r<n;++r)
					s+=a[r][j];
				ans=max(ans,s);
			}
	if(ans<0)
		return string("Bad Game!");
	return to_string(ans);
}

int main()
{
	mt19937 rn(20241022);
	for(int t=1;t<=3000;++t)
	{
		int n=rn()%8+1;
		int p0=rn()%100;
		vector<vector<int>>g(n,vector<int>(n));
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				g[i][j]=(int)(rn()%100)<p0?0:(int)(rn()%6);
		string r1=fast(n,g),r2=brute(n,g);
		if(r1!=r2)
		{
			cerr<<"Mismatch at test "<<t<<": fast="<<r1<<" brute="<<r2<<'\n';
			cerr<<n<<'\n';
			for(int i=0;i<n;++i)
			{
				for(int j=0;j<n;++j)
					cerr<<g[i][j]<<' ';
				cerr<<'\n';
			}
			return 1;
		}
	}
	cout<<"all 3000 tests passed\n";
	return 0;
}
