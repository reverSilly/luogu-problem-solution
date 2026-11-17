// 题目：P3028 [USACO10OCT] Soda Machine G
// 扫描线：闭区间 [A,B] 拆成 (A,+1),(B+1,-1)，排序后扫运行和取 max
// pair 默认字典序：同坐标 -1 排在 +1 前 = 先撤走再进场，杜绝相邻区间的瞬时虚高
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n;
pair<int,int>e[mxn*2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,a,b;i<n;++i)
	{
		cin>>a>>b;
		e[2*i]={a,1};
		e[2*i+1]={b+1,-1};	// 闭区间：B 处仍覆盖，B+1 才撤（b+1<=1e9+1 不爆 int）
	}
	sort(e,e+2*n);
	int s=0,ans=0;
	for(int t=0;t<2*n;++t)
	{
		s+=e[t].second;
		ans=max(ans,s);
	}
	cout<<ans<<'\n';
	return 0;
}
