// 题目：P3068 [USACO13JAN] Party Invitations S
// 闭包模拟：cnt[g] 记录朋友圈 g 中已邀请数，达到 k-1 就邀请剩下一头
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005},mxm{250005};

int n,g,sz[mxm],gs[mxm],cnt[mxm],mem[mxm];
int H[mxn],tot;
struct{int g,nx;}E[mxm];	// 牛 -> 所属朋友圈 的链式前向星
bool inv[mxn];
int q[mxn],qh,qt,ans;

void invite(int v)
{
	if(inv[v])
		return;
	inv[v]=true;
	q[qt++]=v;
	++ans;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>g;
	fill_n(H,n,-1);	// 全局默认 0 是合法边下标，必须显式置 -1
	int mp=0;
	for(int i=0,k;i<g;++i)
	{
		cin>>k;
		sz[i]=k;gs[i]=mp;
		for(int j=0,v;j<k;++j)
		{
			cin>>v;--v;
			mem[mp++]=v;
			E[tot]={i,H[v]};H[v]=tot++;
		}
	}
	invite(0);
	for(int i=0;i<g;++i)	// k=1 的圈：0 人邀请即满足 k-1，BFS 永远等不到它
		if(sz[i]==1)
			invite(mem[gs[i]]);
	while(qh<qt)
	{
		int u=q[qh++];
		for(int i=H[u];i!=-1;i=E[i].nx)
		{
			int t=E[i].g;
			++cnt[t];
			if(cnt[t]==sz[t]-1)	// 每圈至多触发一次：触发后 cnt 直奔 k
				for(int j=gs[t];j<gs[t]+sz[t];++j)
					if(!inv[mem[j]])
						invite(mem[j]);
		}
	}
	cout<<ans<<'\n';
	return 0;
}
