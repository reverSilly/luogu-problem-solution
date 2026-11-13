// 题目：P2392 kkksc03 考前临时抱佛脚
// 每科把题目分成两堆交给左右脑，用时 max(堆和)；01 背包求 <=S/2 的最大子集和
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1205},mxs{25};	// 单科总时长 <=20*60=1200

int s[4],a[mxs],tot,half;
bool dp[mxn];				// dp[j]=能否选出若干道题恰好凑出 j 分钟

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<4;++i)
		cin>>s[i];
	int ans=0;
	for(int k=0;k<4;++k)
	{
		tot=0;
		for(int i=0;i<s[k];++i)
		{
			cin>>a[i];
			tot+=a[i];
		}
		half=tot/2;
		fill_n(dp,mxn,false);	// 多科之间必须清空，全局 bool 默认 0 不是「上一科的结果」
		dp[0]=true;
		for(int i=0;i<s[k];++i)
			for(int j=half;j>=a[i];--j)
				if(dp[j-a[i]])
					dp[j]=true;
		int best=0;
		for(int j=half;j>=0;--j)
			if(dp[j])
			{
				best=j;
				break;
			}
		ans+=tot-best;		// best<=tot/2 ⇒ max(best,tot-best)=tot-best
	}
	cout<<ans<<'\n';
	return 0;
}
