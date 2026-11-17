// 题目：P3029 [USACO11NOV] Cow Lineup S
// 按 x 排序 + 品种离散化，尺取法找覆盖全部品种的最短窗口
// 合法左端是前缀、L(r) 单调不减 ⇒ 双指针只进不退；have==tot 即全覆盖
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};
constexpr int inf{1145141919};

struct cow{int x,b;};

int n,tot,bs[mxn],cnt[mxn];
cow a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i].x>>a[i].b;
		bs[i]=a[i].b;
	}
	sort(a,a+n,[](cow u,cow v){return u.x<v.x;});
	sort(bs,bs+n);
	tot=unique(bs,bs+n)-bs;
	for(int i=0;i<n;++i)
		a[i].b=lower_bound(bs,bs+tot,a[i].b)-bs;	// 品种只判等，离散化不丢信息
	int l=0,have=0,ans=inf;	// ans 必须 INF：0 是合法代价，不能靠全局默认
	for(int r=0;r<n;++r)
	{
		if(cnt[a[r].b]++==0)
			++have;
		while(have==tot)
		{
			ans=min(ans,a[r].x-a[l].x);
			if(--cnt[a[l].b]==0)
				--have;
			++l;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
