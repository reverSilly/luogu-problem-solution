// 题目：P2681 众数
// 思路：n,m<=1000 ⇒ 每次询问把区间拷出来排序，扫相等段计频，O(m*n log n) 足够。
//       值域 1e9 不能开桶；排序天然把相同值聚成段，且段按值升序出现，
//       计频时用严格 > 更新答案，就自动实现"众数并列取较小值"。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,m,a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int q=0,flag,x,y;q<m;++q)
	{
		cin>>flag>>x>>y;--x;
		if(flag)
			a[x]=y;	// 修改：y 是值，不是下标，不能 --
		else
		{
			--y;
			int len=y-x+1;
			for(int i=0;i<len;++i)
				b[i]=a[x+i];
			sort(b,b+len);
			int ans=b[0],mx=1,cnt=1;
			for(int i=1;i<len;++i)
			{
				cnt=(b[i]==b[i-1]?cnt+1:1);
				if(cnt>mx)	// 严格大于：并列时保留先出现的（更小的）值
					mx=cnt,ans=b[i];
			}
			cout<<ans<<'\n';
		}
	}
	return 0;
}