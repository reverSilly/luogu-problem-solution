//
// 题目：P1565 牛宫
// https://www.luogu.com.cn/problem/P1565
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};          // N,M<=200 (+5 松弛)

int n,m;
i64 a[mxn][mxn];                 // 海拔(可低至 -2e9,列和/前缀需 64 位)
i64 c[mxn];                      // 当前行区间各列的和(滚动复用)
i64 p[mxn];                      // c 的前缀和 p[0..m]
int stk[mxn];                    // 严格递减前缀最小值的下标单调栈

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>a[i][j];
	int ans=0;                   // 不存在正和子矩阵时输出 0
	for(int top=0;top<n;++top)
	{
		for(int j=0;j<m;++j)
			c[j]=0;
		for(int bot=top;bot<n;++bot)
		{
			for(int j=0;j<m;++j)
				c[j]+=a[bot][j];
			p[0]=0;
			for(int j=0;j<m;++j)
				p[j+1]=p[j]+c[j];
			int tp=0;            // 栈内值 p[stk[*]] 严格递减
			stk[tp++]=0;
			int best=0;          // 本行区间下最长正和子数组长度
			for(int r=1;r<=m;++r)
			{
				// 二分找栈中下标最小且 p[l]<p[r] 的 l
				int lo=0,hi=tp-1,pos=-1;
				while(lo<=hi)
				{
					int mid=(lo+hi)>>1;
					if(p[stk[mid]]<p[r])
					{
						pos=stk[mid];
						hi=mid-1;
					}
					else
						lo=mid+1;
				}
				if(pos!=-1&&r-pos>best)
					best=r-pos;
				if(p[r]<p[stk[tp-1]])
					stk[tp++]=r;
			}
			int h=bot-top+1;
			if(best*h>ans)
				ans=best*h;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
