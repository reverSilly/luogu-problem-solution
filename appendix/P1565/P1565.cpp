//
// 题目：P1565 牛宫
// https://www.luogu.com.cn/problem/P1565
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};           // N,M<=200 (+5 松弛)

int n,m;
long long a[mxn][mxn],col_sum[mxn],pref[mxn];
int stk[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>a[i][j];

	int max_area=0;
	for(int r1=0;r1<n;++r1)
	{
		memset(col_sum,0,sizeof(long long)*m);
		for(int r2=r1;r2<n;++r2)
		{
			for(int c=0;c<m;++c)
				col_sum[c]+=a[r2][c];

			pref[0]=0;
			for(int c=0;c<m;++c)
				pref[c+1]=pref[c]+col_sum[c];

			// 单调递减栈存左端点下标 l in [0, m-1]
			int top=0;
			for(int l=0;l<m;++l)
				if(top==0||pref[l]<pref[stk[top-1]])
					stk[top++]=l;

			int best_w=0;
			for(int r=m;r>=1;--r)
				while(top>0&&pref[r]>pref[stk[top-1]])
				{
					best_w=max(best_w,r-stk[top-1]);
					--top;
				}

			max_area=max(max_area,(r2-r1+1)*best_w);
		}
	}

	cout<<max_area<<'\n';
	return 0;
}
