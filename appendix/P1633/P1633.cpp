//
// 题目：P1633 二进制
// https://www.luogu.com.cn/problem/P1633
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxl{33};
constexpr i64 inf{1145141919810114514LL};

i64 dp[mxl][mxl][mxl][2],ndp[mxl][mxl][mxl][2];

void solve()
{
	i64 A,B,C;
	cin>>A>>B>>C;
	int L=max({__lg(A),__lg(B),__lg(C)})+1;
	int ca=__builtin_popcountll(A);
	int cb=__builtin_popcountll(B);
	int cc=__builtin_popcountll(C);

	for(int a=0;a<=ca;++a)
		for(int b=0;b<=cb;++b)
			for(int c=0;c<=cc;++c)
				dp[a][b][c][0]=dp[a][b][c][1]=inf;
	dp[0][0][0][0]=0;

	for(int i=0;i<L;++i)
	{
		int ma=min(i+1,ca),mb=min(i+1,cb),mc=min(i+1,cc);
		for(int a=0;a<=ma;++a)
			for(int b=0;b<=mb;++b)
				for(int c=0;c<=mc;++c)
					ndp[a][b][c][0]=ndp[a][b][c][1]=inf;

		int pa=min(i,ca),pb=min(i,cb),pc=min(i,cc);
		i64 bit=1LL<<i;
		for(int a=0;a<=pa;++a)
			for(int b=0;b<=pb;++b)
				for(int c=0;c<=pc;++c)
					for(int carry=0;carry<2;++carry)
					{
						i64 cur=dp[a][b][c][carry];
						if(cur==inf)
							continue;
						for(int x=0;x<2;++x)
						{
							if(a+x>ca)
								continue;
							for(int y=0;y<2;++y)
							{
								if(b+y>cb)
									continue;
								int s=x+y+carry;
								int z=s&1,nc=s>>1;
								if(c+z>cc)
									continue;
								i64 val=cur+(z?bit:0);
								if(val<ndp[a+x][b+y][c+z][nc])
									ndp[a+x][b+y][c+z][nc]=val;
							}
						}
					}

		for(int a=0;a<=ma;++a)
			for(int b=0;b<=mb;++b)
				for(int c=0;c<=mc;++c)
				{
					dp[a][b][c][0]=ndp[a][b][c][0];
					dp[a][b][c][1]=ndp[a][b][c][1];
				}
	}

	i64 ans=dp[ca][cb][cc][0];
	cout<<(ans==inf?-1:ans)<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
		solve();
	return 0;
}
