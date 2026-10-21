//
// P1613 暴力（对拍用）
// 关键等价：把一条长 L 的行走切成若干段 2^{k_i} 边, 秒数 = 段数；
// L 能写成 t 个 2 的幂之和当且仅当 popcount(L) <= t <= L, 故一条长度 L 的行走最少要 popcount(L) 秒。
// 于是答案 = 所有"可达的行走长度 L"中最小的 popcount(L)。这里枚举 L <= 2^18（位运算推进可达集）
#include<bits/stdc++.h>
using namespace std;
using u64=unsigned long long;

constexpr int LIM{1<<18};

int n,m;
u64 adj[64];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;
		--u;--v;
		adj[u]|=1ull<<v;
	}
	u64 cur=1;                       // 走 0 条边时只在 0 号点
	int best=100;
	for(int L=1;L<=LIM&&best>1;++L)
	{
		u64 nxt=0;
		for(int u=0;u<n;++u)
			if(cur>>u&1)
				nxt|=adj[u];
		cur=nxt;
		if(nxt>>(n-1)&1)
			best=min(best,__builtin_popcount((unsigned)L));
	}
	if(best==100)
		cout<<-1<<'\n';
	else
		cout<<best<<'\n';
	return 0;
}
