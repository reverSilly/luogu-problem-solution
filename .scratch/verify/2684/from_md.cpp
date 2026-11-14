// 题目：P2684 搞清洁（USACO Cleaning Shifts）
// 最小区间覆盖：cur = 最左未覆盖时间段，在 l<=cur 的区间里取 r 最大者，cur=r+1
// 排序 + 单调指针：cur 递增 ⇒ 旧候选的 r 恒 < cur，永远不用回头扫
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25005};
constexpr int inf{1145141919};

int n,t,ans;
struct seg{int l,r;}a[mxn];

void upd(int&x,int v){x=max(x,v);}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>t;
	for(int i=0;i<n;++i)
	{
		cin>>a[i].l>>a[i].r;
		--a[i].l;--a[i].r;	// 内部 0-based：时间段 0..t-1
	}
	sort(a,a+n,[](seg x,seg y){return x.l<y.l;});
	int cur=0,i=0;	// cur=最左未覆盖点；i=永不回退的扫描指针
	while(cur<t)
	{
		int mx=-inf;	// 本轮候选的最大右端点；旧候选恒废，不能沿用
		while(i<n&&a[i].l<=cur)
			upd(mx,a[i].r),++i;
		if(mx<cur)	// 没有任何区间能盖住 cur ⇒ 客观无解
		{
			cout<<-1<<'\n';
			return 0;
		}
		cur=mx+1;++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
