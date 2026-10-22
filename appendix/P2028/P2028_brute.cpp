// 题目：P2028 龙兄摘苹果
// 暴力：枚举 n 个元素的所有「受限增长串」（集合划分），统计恰好 k 个块的方案数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using u64=unsigned long long;

constexpr int mxn{15};

int n,k;
u64 p;
u64 f[mxn];
int cur[mxn];

void dfs(int i,int mx,int blk)
{
	if(i==n)
	{
		if(blk==k)
			++f[blk];
		return;
	}
	for(int v=1;v<=mx+1;++v)
	{
		cur[i]=v;
		dfs(i+1,max(mx,v),blk+(v==mx+1));
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>p;
	dfs(0,0,0);
	//f[blk] 记录块数为 blk 的划分数，这里只关心 k
	//由于 dfs 内按 blk 累加到 f[blk]，f[k] 即为 k 块划分数
	u64 cnt=f[k];
	cout<<(cnt%p)<<'\n';
	return 0;
}
