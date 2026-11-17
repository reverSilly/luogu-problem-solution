// 题目：P2889 Milking Time S
// 带休息间隔的加权区间调度：按结束时间排序，f[i]=只考虑前 i+1 段时的最优产奶量
// f[i]=max(f[i-1], w[i]+f[j])，j 是最后一段满足 e[j]+R<=s[i] 的区间（二分）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

struct node{int s,e,w;}a[mxn];
int n,m,r,e[mxn];
i64 f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>r;
	for(int i=0;i<m;++i)
		cin>>a[i].s>>a[i].e>>a[i].w;
	sort(a,a+m,[](node x,node y){return x.e<y.e;});
	for(int i=0;i<m;++i)
		e[i]=a[i].e;
	for(int i=0;i<m;++i)
	{
		// e[j]<=s[i]-r<e[i]，故兼容区间恰为前缀 [0,j]，二分只查 [0,i)
		int j=upper_bound(e,e+i,a[i].s-r)-e-1;
		i64 take=a[i].w+(j>=0?f[j]:0);
		f[i]=max(i?f[i-1]:0LL,take);	// 不选第 i 段 / 选第 i 段
	}
	cout<<f[m-1]<<'\n';
	return 0;
}
