// 题目：P3016 [USACO11FEB] The Triangle S
// 枚举所有行数 >=K 的正立/倒立子三角形，行前缀和 + 逐层增量使每个三角形 O(1)
// 答案 = 各三角形平均值向零截断后的最大值；C++ 整除恰为向零截断
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{705};

int n,k;
i64 pre[mxn][mxn],cnt[mxn],ans=-1145141919810;	// pre[i][j]=第 i 行前 j 个数之和；cnt[s]=s(s+1)/2

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		for(int j=0,v;j<=i;++j)
		{
			cin>>v;
			pre[i][j+1]=pre[i][j]+v;
		}
	for(int s=1;s<=n;++s)
		cnt[s]=cnt[s-1]+s;
	// 正立：顶点 (i,j)，第 s 层新增行 i+s-1 的 [j,j+s-1]
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
		{
			i64 cur=0;
			for(int s=1;i+s<=n;++s)
			{
				cur+=pre[i+s-1][j+s]-pre[i+s-1][j];
				if(s<k)
					continue;
				i64 c=cnt[s];
				// 截断单调：仅当 trunc(cur/c)>ans 才做除法（负数分支判据不同！）
				if(cur>=0?cur>=(ans+1)*c:cur>ans*c)
					ans=cur/c;
			}
		}
	// 倒立：底点 (i,j)，第 s 层新增行 i-s+1 的 [j-s+1,j]
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
		{
			i64 cur=0;
			for(int s=1,mx=min(j,i-j)+1;s<=mx;++s)
			{
				cur+=pre[i-s+1][j+1]-pre[i-s+1][j-s+1];
				if(s<k)
					continue;
				i64 c=cnt[s];
				if(cur>=0?cur>=(ans+1)*c:cur>ans*c)
					ans=cur/c;
			}
		}
	cout<<ans<<'\n';
	return 0;
}
