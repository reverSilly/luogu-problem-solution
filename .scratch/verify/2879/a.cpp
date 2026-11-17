// 题目：P2879 [USACO07JAN] Tallest Cow S
// 每条去重后的关系 (l,r) 令开区间 (l,r) 内各减 1 ⇒ 差分；答案 H-被覆盖次数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,I,H,R,d[mxn];
pair<int,int>rel[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>I>>H>>R;--I;
	for(int i=0,a,b;i<R;++i)
	{
		cin>>a>>b;--a;--b;
		if(a>b)
			swap(a,b);
		rel[i]={a,b};
	}
	sort(rel,rel+R);
	R=unique(rel,rel+R)-rel;	// 同一对牛只算一次，重复信息不叠加
	for(int i=0;i<R;++i)
	{
		auto[l,r]=rel[i];
		--d[l+1];++d[r];	// 开区间 (l,r) 各减 1；相邻时 l+1==r 自动抵消
	}
	int c=0;
	for(int i=0;i<n;++i)
	{
		c+=d[i];
		cout<<H+c<<'\n';
	}
	return 0;
}
