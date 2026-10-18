//
// 题目：P1552 [APIO2012] 派遣
// https://www.luogu.com.cn/problem/P1552
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // N<=10^5 (+5 松弛)

int n;
long long m;
int b[mxn],l_val[mxn];
long long c[mxn];

// 左偏树(大根堆)维护子树内薪水最小的若干忍者
int rt[mxn],lc[mxn],rc[mxn],dist_arr[mxn],sz[mxn];
long long sum_c[mxn];

int merge_heap(int x,int y)
{
	if(!x||!y)
		return x|y;
	if(c[x]<c[y])
		swap(x,y);
	rc[x]=merge_heap(rc[x],y);
	if(dist_arr[lc[x]]<dist_arr[rc[x]])
		swap(lc[x],rc[x]);
	dist_arr[x]=dist_arr[rc[x]]+1;
	return x;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=1;i<=n;++i)
	{
		cin>>b[i]>>c[i]>>l_val[i];
		rt[i]=i;
		sz[i]=1;
		sum_c[i]=c[i];
	}

	long long ans=0;
	// 题面保证 B_i < i, 故逆序 i = n..1 即为严格自底向上的拓扑序!
	for(int i=n;i>=1;--i)
	{
		while(sum_c[i]>m)
		{
			sum_c[i]-=c[rt[i]];
			--sz[i];
			rt[i]=merge_heap(lc[rt[i]],rc[rt[i]]);
		}
		ans=max(ans,1LL*l_val[i]*sz[i]);
		if(b[i]>0)
		{
			int p=b[i];
			rt[p]=merge_heap(rt[p],rt[i]);
			sz[p]+=sz[i];
			sum_c[p]+=sum_c[i];
		}
	}

	cout<<ans<<'\n';
	return 0;
}
