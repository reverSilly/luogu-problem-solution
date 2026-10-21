//
// 题目：P1558 色板游戏
// https://www.luogu.com.cn/problem/P1558
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // L<=100000 (+5 松弛)

int L,T,O;
int tr[mxn<<2],lz[mxn<<2];

void build(int u,int l,int r)
{
	lz[u]=0;
	tr[u]=1<<0;                   // 初始全为 1 号颜色 (0-based 第 0 位)
	if(l==r)
		return;
	int mid=(l+r)>>1;
	build(u<<1,l,mid);
	build(u<<1|1,mid+1,r);
}

void apply(int u,int mask)
{
	tr[u]=mask;
	lz[u]=mask;
}

void pushdown(int u)
{
	if(lz[u])
	{
		apply(u<<1,lz[u]);
		apply(u<<1|1,lz[u]);
		lz[u]=0;
	}
}

void update(int u,int l,int r,int ql,int qr,int mask)
{
	if(ql<=l&&r<=qr)
	{
		apply(u,mask);
		return;
	}
	pushdown(u);
	int mid=(l+r)>>1;
	if(ql<=mid)
		update(u<<1,l,mid,ql,qr,mask);
	if(qr>mid)
		update(u<<1|1,mid+1,r,ql,qr,mask);
	tr[u]=tr[u<<1]|tr[u<<1|1];
}

int query(int u,int l,int r,int ql,int qr)
{
	if(ql<=l&&r<=qr)
		return tr[u];
	pushdown(u);
	int mid=(l+r)>>1,res=0;
	if(ql<=mid)
		res|=query(u<<1,l,mid,ql,qr);
	if(qr>mid)
		res|=query(u<<1|1,mid+1,r,ql,qr);
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>L>>T>>O))
		return 0;
	build(1,0,L-1);

	for(int i=0;i<O;++i)
	{
		char op;
		int a,b;
		cin>>op>>a>>b;
		if(a>b)
			swap(a,b);
		--a;--b;
		if(op=='C')
		{
			int c;
			cin>>c;
			--c;
			update(1,0,L-1,a,b,1<<c);
		}
		else
		{
			int mask=query(1,0,L-1,a,b);
			cout<<__builtin_popcount(mask)<<'\n';
		}
	}
	return 0;
}
