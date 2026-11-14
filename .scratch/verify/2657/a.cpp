// 题目：P2657 [SCOI2009] windy 数
// 思路：区间计数拆前缀差 count(b)-count(a-1)。
//       count(x) 用数位 DP：状态 (pos,pre,lead,lim)。
//       windy 约束只耦合相邻位 ⇒ 记上一位 pre 即可。
//       前导零是占位：lead 阶段不查差、pre 归一化成哨兵 10、全零串代表 0 要排除。
//       lim=false 的子问题无后效 ⇒ 记忆化 f[pos][pre][lead]。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};	// 2e9 最多 10 位，+5 松弛

int len,dig[mxn];
i64 f[mxn][11][2];	// f[pos][pre][lead]，pre=10 表示仍在前导零

i64 dfs(int pos,int pre,bool lead,bool lim)
{
	if(pos==len)
		return lead?0:1;	// 全前导零代表 0，不是 windy 数
	if(!lim&&f[pos][pre][lead]!=-1)
		return f[pos][pre][lead];
	int up=lim?dig[pos]:9;
	i64 res=0;
	for(int d=0;d<=up;++d)
	{
		bool nlim=lim&&d==dig[pos];
		if(lead&&d==0)
			res+=dfs(pos+1,10,true,nlim);	// 继续占位，不查差
		else
		{
			if(!lead&&abs(d-pre)<2)
				continue;	// 相邻差 <2，非法
			res+=dfs(pos+1,d,false,nlim);
		}
	}
	if(!lim)
		f[pos][pre][lead]=res;	// 只记忆自由状态
	return res;
}

i64 count(i64 x)
{
	if(x<=0)
		return 0;
	len=0;
	for(i64 t=x;t;t/=10)
		dig[len++]=t%10;
	for(int i=0;i<len/2;++i)
		swap(dig[i],dig[len-1-i]);	// 翻成高位在前
	memset(f,-1,sizeof f);	// i64 全 0xFF = -1，作"未计算"标记
	return dfs(0,10,true,true);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 a,b;
	cin>>a>>b;
	cout<<count(b)-count(a-1)<<'\n';
	return 0;
}
