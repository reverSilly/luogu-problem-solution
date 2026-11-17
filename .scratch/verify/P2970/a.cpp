// 题目：P2970 [USACO09DEC] Selfish Grazing S
// 经典活动选择：按右端点排序，能选就选（端点相接合法）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n,ans;
struct seg{int s,e;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].s>>a[i].e;
	sort(a,a+n,[](seg x,seg y){return x.e<y.e;});
	int cur{-114514};	// 已选最后一头的 E；S_i>=1，哨兵保证第一头必可选
	for(int i=0;i<n;++i)
		if(a[i].s>=cur)	// 相接合法：S>=cur 即可，不是 S>cur
		{
			++ans;
			cur=a[i].e;
		}
	cout<<ans<<'\n';
	return 0;
}