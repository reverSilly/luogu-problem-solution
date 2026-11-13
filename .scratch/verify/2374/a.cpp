// 题目：P2374 搬运工
// f[a][b][c]：三堆分别已经取走 a,b,c 本时的最大消耗；此时体力系数 t=a+b+c，
// 最后一次取的书只能来自某一堆的顶部，三种来源取 max
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int A,B,C;
int w[3][mxn];        //w[t][1..n_t] 由下到上
int f[mxn][mxn][mxn]; //f[a][b][c]=前三堆各取 a,b,c 本的最大消耗

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B>>C;
	for(int i=1;i<=A;++i)cin>>w[0][i];
	for(int i=1;i<=B;++i)cin>>w[1][i];
	for(int i=1;i<=C;++i)cin>>w[2][i];
	for(int a=0;a<=A;++a)
		for(int b=0;b<=B;++b)
			for(int c=0;c<=C;++c)
			{
				int t=a+b+c;
				if(t==0)
					continue;
				int best=-1;
				if(a)best=max(best,f[a-1][b][c]+t*w[0][A-a+1]);
				if(b)best=max(best,f[a][b-1][c]+t*w[1][B-b+1]);
				if(c)best=max(best,f[a][b][c-1]+t*w[2][C-c+1]);
				f[a][b][c]=best;
			}
	cout<<f[A][B][C]<<'\n';
	return 0;
}
