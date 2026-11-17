// 独立暴力：枚举所有连续分段（<=k 段），每段学校位置枚举所有楼的坐标取最小
// 不依赖中位数结论与前缀和公式，直接按定义算
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,k;
i64 a[15],x[15];
i64 best;
i64 gcost(int l,int r)
{
	i64 mn=LLONG_MAX;
	for(int p=0;p<n;++p)
	{
		i64 c=0;
		for(int t=l;t<=r;++t)
			c+=a[t]*llabs(x[t]-x[p]);
		mn=min(mn,c);
	}
	return mn;
}
void dfs(int pos,int used,i64 cost)
{
	if(pos==n)
	{
		best=min(best,cost);
		return;
	}
	if(used==k)
		return;
	for(int e=pos;e<n;++e)
		dfs(e+1,used+1,cost+gcost(pos,e));
}
int main()
{
	cin>>n>>k;
	for(int i=0;i<n;++i)cin>>a[i];
	for(int i=1,d;i<n;++i){cin>>d;x[i]=x[i-1]+d;}
	best=LLONG_MAX;
	dfs(0,0,0);
	cout<<best<<'\n';
	return 0;
}