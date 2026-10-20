//
// 题目：P1568 赛跑
// https://www.luogu.com.cn/problem/P1568
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};          // N,M<=1000 (+5 松弛)
constexpr int mxt{1000005};       // 总时间上界 1000*1000 (+5 松弛)

int n,m;
int sv[mxn],st[mxn],kv[mxn],kt[mxn];
i64 ps[mxt],pk[mxt];              // ps[t]/pk[t]=第 t 个单位时间末的位置

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>sv[i]>>st[i];
	for(int i=0;i<m;++i)
		cin>>kv[i]>>kt[i];
	int ts=0,tk=0;
	ps[0]=0;
	for(int i=0;i<n;++i)
		for(int e=0;e<st[i];++e)
		{
			++ts;                 // 先自增再读 ps[ts-1],避免 ++ 与索引的求值顺序陷阱
			ps[ts]=ps[ts-1]+sv[i];
		}
	pk[0]=0;
	for(int i=0;i<m;++i)
		for(int e=0;e<kt[i];++e)
		{
			++tk;
			pk[tk]=pk[tk-1]+kv[i];
		}
	int T=min(ts,tk);
	int cnt=0,prv=0;              // prv=上一次已确立的领先者(+1 SH / -1 KC)
	for(int t=1;t<=T;++t)
	{
		int lead=(ps[t]>pk[t])-(ps[t]<pk[t]);
		if(lead!=0)               // 平局不改变领先者,直接跳过
		{
			if(prv!=0&&prv!=lead)
				++cnt;
			prv=lead;
		}
	}
	cout<<cnt<<'\n';
	return 0;
}
