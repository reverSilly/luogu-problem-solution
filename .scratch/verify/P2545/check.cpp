#include<bits/stdc++.h>
using namespace std;
using i64=long long;
constexpr i64 ninf{-(1LL<<60)};

i64 dp(int n,const vector<i64>&top,const vector<i64>&bot)
{
	vector<i64>a(n+1,0),c(n+1,0);
	for(int i=1;i<=n;++i)a[i]=a[i-1]+top[i];
	for(int i=1;i<=n;++i)c[i]=c[i-1]+top[i]+bot[i];
	i64 d1=ninf,d2=ninf,d3=ninf,ans=ninf;
	for(int i=0;i<=n;++i)
	{
		if(d3>ninf/2)ans=max(ans,d3+c[i]);
		if(d2>ninf/2)d3=max(d3,d2-a[i]);
		if(d1>ninf/2)d2=max(d2,d1+a[i]);
		d1=max(d1,-c[i]);
	}
	return ans;
}
i64 brute(int n,const vector<i64>&top,const vector<i64>&bot)
{
	vector<i64>A(n+1,0),B(n+1,0);
	for(int i=1;i<=n;++i){A[i]=A[i-1]+top[i];B[i]=B[i-1]+bot[i];}
	i64 best=ninf;
	for(int l=1;l<=n;++l)
		for(int r=l+2;r<=n;++r)
			for(int a=l+1;a<=r-1;++a)
				for(int b=a;b<=r-1;++b)
				{
					i64 v=(B[r]-B[l-1])+(A[a-1]-A[l-1])+(A[r]-A[b]);
					best=max(best,v);
				}
	return best;
}
int main()
{
	mt19937 rn(12345);
	int bad=0;
	for(int t=0;t<20000;++t)
	{
		int n=4+rn()%6;	// 4..9
		vector<i64>top(n+1),bot(n+1);
		for(int i=1;i<=n;++i){top[i]=(i64)(rn()%21)-10;bot[i]=(i64)(rn()%21)-10;}
		i64 x=dp(n,top,bot),y=brute(n,top,bot);
		if(x!=y)
		{
			++bad;
			if(bad<=5)
			{
				cerr<<"MISMATCH n="<<n<<" dp="<<x<<" brute="<<y<<"\n top:";
				for(int i=1;i<=n;++i)cerr<<' '<<top[i];
				cerr<<"\n bot:";
				for(int i=1;i<=n;++i)cerr<<' '<<bot[i];
				cerr<<'\n';
			}
		}
	}
	// 边界：全负、全正、单列极值
	cerr<<"done, mismatches="<<bad<<"\n";
	return 0;
}