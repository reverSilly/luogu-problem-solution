//
// 题目：P1600 [NOIP 2016 提高组] 天天爱跑步
// https://www.luogu.com.cn/problem/P1600
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};        // n,m<=300000 (+5 松弛)
constexpr int lg{19};

int n,m;
int w[mxn],dep[mxn],up[lg][mxn],q_bfs[mxn];
int ans[mxn];

// 链式前向星存树边
int head_e[mxn],to_e[mxn<<1],nxt_e[mxn<<1],ecnt;

// 链式前向星存每个节点上的差分事件
int head_up_add[mxn],val_up_add[mxn],nxt_up_add[mxn],cnt_ua;
int head_up_del[mxn],val_up_del[mxn],nxt_up_del[mxn],cnt_ud;
int head_dn_add[mxn],val_dn_add[mxn],nxt_dn_add[mxn],cnt_da;
int head_dn_del[mxn],val_dn_del[mxn],nxt_dn_del[mxn],cnt_dd;

int bucket_up[mxn<<1];            // 桶维护上行路径 dep[s]
int bucket_dn[mxn<<1];            // 桶维护下行路径 dep[s] - 2*dep[lca] + n

void add_edge(int u,int v)
{
	to_e[++ecnt]=v;nxt_e[ecnt]=head_e[u];head_e[u]=ecnt;
	to_e[++ecnt]=u;nxt_e[ecnt]=head_e[v];head_e[v]=ecnt;
}

void push_ua(int u,int v){val_up_add[++cnt_ua]=v;nxt_up_add[cnt_ua]=head_up_add[u];head_up_add[u]=cnt_ua;}
void push_ud(int u,int v){val_up_del[++cnt_ud]=v;nxt_up_del[cnt_ud]=head_up_del[u];head_up_del[u]=cnt_ud;}
void push_da(int u,int v){val_dn_add[++cnt_da]=v;nxt_dn_add[cnt_da]=head_dn_add[u];head_dn_add[u]=cnt_da;}
void push_dd(int u,int v){val_dn_del[++cnt_dd]=v;nxt_dn_del[cnt_dd]=head_dn_del[u];head_dn_del[u]=cnt_dd;}

void bfs_lca()
{
	memset(dep,-1,sizeof(int)*n);
	int ql=0,qr=0;
	dep[0]=0;
	up[0][0]=0;
	q_bfs[qr++]=0;
	while(ql<qr)
	{
		int u=q_bfs[ql++];
		for(int k=1;k<lg;++k)
			up[k][u]=up[k-1][up[k-1][u]];
		for(int e=head_e[u];e;e=nxt_e[e])
		{
			int v=to_e[e];
			if(dep[v]==-1)
			{
				dep[v]=dep[u]+1;
				up[0][v]=u;
				q_bfs[qr++]=v;
			}
		}
	}
}

int get_lca(int u,int v)
{
	if(dep[u]<dep[v])
		swap(u,v);
	int diff=dep[u]-dep[v];
	for(int k=0;k<lg;++k)
		if((diff>>k)&1)
			u=up[k][u];
	if(u==v)
		return u;
	for(int k=lg-1;k>=0;--k)
		if(up[k][u]!=up[k][v])
		{
			u=up[k][u];
			v=up[k][v];
		}
	return up[0][u];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n-1;++i)
	{
		int u,v;
		cin>>u>>v;
		--u;--v;
		add_edge(u,v);
	}
	for(int i=0;i<n;++i)
		cin>>w[i];

	bfs_lca();

	for(int i=0;i<m;++i)
	{
		int s,t;
		cin>>s>>t;
		--s;--t;
		int g=get_lca(s,t);
		int v_up=dep[s];
		int v_dn=dep[s]-2*dep[g]+n;
		push_ua(s,v_up);
		push_ud(g,v_up);
		push_da(t,v_dn);
		push_dd(g,v_dn);
		// LCA 节点 g 同时被上行与下行路径覆盖, 若恰好在 g 被观察到需去重一次
		if(dep[s]-dep[g]==w[g])
			--ans[g];
	}

	// 非递归显式栈实现欧拉序子树差分统计, 彻底规避极限链爆栈
	vector<pair<int,int>> stk;
	stk.reserve(n*2);
	stk.push_back({0,0});
	while(!stk.empty())
	{
		auto [u,phase]=stk.back();
		stk.pop_back();
		int ku=dep[u]+w[u];
		int kd=w[u]-dep[u]+n;
		if(phase==0)
		{
			if(ku<=n)
				ans[u]-=bucket_up[ku];
			if(kd>=0&&kd<=2*n)
				ans[u]-=bucket_dn[kd];
			stk.push_back({u,1});
			for(int e=head_e[u];e;e=nxt_e[e])
			{
				int v=to_e[e];
				if(v!=up[0][u])
					stk.push_back({v,0});
			}
		}
		else
		{
			for(int e=head_up_add[u];e;e=nxt_up_add[e])
				++bucket_up[val_up_add[e]];
			for(int e=head_dn_add[u];e;e=nxt_dn_add[e])
				++bucket_dn[val_dn_add[e]];
			if(ku<=n)
				ans[u]+=bucket_up[ku];
			if(kd>=0&&kd<=2*n)
				ans[u]+=bucket_dn[kd];
			for(int e=head_up_del[u];e;e=nxt_up_del[e])
				--bucket_up[val_up_del[e]];
			for(int e=head_dn_del[u];e;e=nxt_dn_del[e])
				--bucket_dn[val_dn_del[e]];
		}
	}

	for(int i=0;i<n;++i)
		cout<<ans[i]<<(i+1==n?'\n':' ');
	return 0;
}
