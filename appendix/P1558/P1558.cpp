//
// 题目：P1558 色板游戏
// https://www.luogu.com.cn/problem/P1558
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // L,O <= 1e5 (+5 松弛)

int L,T,O;
int seg[mxn*4];                   // 每段"出现过的颜色集合"的位掩码(0-based 颜色占第 c 位)
int tag[mxn*4];                   // 懒标记:整段统一涂的颜色(0-based),-1 表示无

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

void build(int l,int r,int i)
{
	seg[i]=1;                     // 初值整块都是 1 号色 => 第 0 位
	tag[i]=-1;
	if(r-l==1)
		return;
	int mid=(l+r)>>1;
	build(l,mid,ls(i));
	build(mid,r,rs(i));
}
void push_down(int i)
{
	if(tag[i]<0)
		return;
	int t=tag[i];
	seg[ls(i)]=seg[rs(i)]=1<<t;
	tag[ls(i)]=tag[rs(i)]=t;
	tag[i]=-1;
}
void upd(int Lq,int Rq,int c,int l,int r,int i)
{
	if(Rq<=l||r<=Lq)
		return;
	if(Lq<=l&&r<=Rq)              // [l,r) 被完全覆盖:整段改色
	{
		seg[i]=1<<c;
		tag[i]=c;
		return;
	}
	push_down(i);
	int mid=(l+r)>>1;
	upd(Lq,Rq,c,l,mid,ls(i));
	upd(Lq,Rq,c,mid,r,rs(i));
	seg[i]=seg[ls(i)]|seg[rs(i)];
}
int qry(int Lq,int Rq,int l,int r,int i)
{
	if(Rq<=l||r<=Lq)
		return 0;
	if(Lq<=l&&r<=Rq)
		return seg[i];
	push_down(i);
	int mid=(l+r)>>1;
	return qry(Lq,Rq,l,mid,ls(i))|qry(Lq,Rq,mid,r,rs(i));
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>L>>T>>O;
	build(0,L,0);
	for(int i=0,op,a,b,c;i<O;++i)
	{
		char ch;
		cin>>ch;
		if(ch=='C')
		{
			cin>>a>>b>>c;
			--a;--b;--c;
			if(a>b)
				swap(a,b);
			upd(a,b+1,c,0,L,0);
		}
		else
		{
			cin>>a>>b;
			--a;--b;
			if(a>b)
				swap(a,b);
			cout<<__builtin_popcount((unsigned)qry(a,b+1,0,L,0))<<'\n';
		}
	}
	return 0;
}
