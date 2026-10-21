//
// 题目：P1607 [USACO09FEB] Fair Shuttle G
// https://www.luogu.com.cn/problem/P1607
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxk{50005},mxn{20005};   // K<=5e4, N<=2e4

int K,N,C,ans;
struct{int s,e,m;}g[mxk];             // 第 i 组: [s,e) 上最多 m 头
int mx[mxn<<2],ad[mxn<<2];            // 区间最大值 + 区间加标记

void push_up(int i){mx[i]=max(mx[i*2+1],mx[i*2+2]);}

void push_down(int i)
{
	if(!ad[i])
		return;
	int a=i*2+1,b=i*2+2;
	mx[a]+=ad[i];ad[a]+=ad[i];
	mx[b]+=ad[i];ad[b]+=ad[i];
	ad[i]=0;
}

void upd(int L,int R,int v,int i,int l,int r)      // [L,R) 加 v, 节点管 [l,r)
{
	if(L<=l&&r<=R)
	{
		mx[i]+=v;ad[i]+=v;
		return;
	}
	push_down(i);
	int mid=(l+r)>>1;
	if(L<mid)
		upd(L,R,v,i*2+1,l,mid);
	if(mid<R)
		upd(L,R,v,i*2+2,mid,r);
	push_up(i);
}

int qry(int L,int R,int i,int l,int r)             // [L,R) 的最大载客量
{
	if(L<=l&&r<=R)
		return mx[i];
	push_down(i);
	int mid=(l+r)>>1,res=0;
	if(L<mid)
		res=max(res,qry(L,R,i*2+1,l,mid));
	if(mid<R)
		res=max(res,qry(L,R,i*2+2,mid,r));
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>K>>N>>C;
	for(int i=0,s,e,m;i<K;++i)
	{
		cin>>s>>e>>m;
		--s;--e;                       // 0-based, 仍满足 s<e
		g[i]={s,e,m};
	}
	sort(g,g+K,[](auto x,auto y){return x.e<y.e;});

	for(int i=0;i<K;++i)
	{
		auto[s,e,m]=g[i];
		int t=min(m,C-qry(s,e,0,0,N-1));   // 全程最空处还能塞几头
		if(t>0)
		{
			ans+=t;
			upd(s,e,t,0,0,N-1);
		}
	}
	cout<<ans<<'\n';
	return 0;
}
