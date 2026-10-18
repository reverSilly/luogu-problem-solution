//
// 题目：P1531 I Hate It
// https://www.luogu.com.cn/problem/P1531
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};        // n,m<=200000 (+5 松弛)
constexpr int inf{1145141919};

int n,m;
int a[mxn],tr[mxn*4];

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];

	// 建树: 半开区间 [l, r), 根下标 0 (sth 模式)
	function<void(int,int,int)>sth{[&sth](int l,int r,int i){
		if(r-l==1)
			tr[i]=a[l];
		else
		{
			int mid=(l+r)>>1;
			sth(l,mid,ls(i));
			sth(mid,r,rs(i));
			tr[i]=max(tr[ls(i)],tr[rs(i)]);
		}
	}};
	sth(0,n,0);

	// 单点前缀高分更新: 仅当 a[pos] < val 时更新
	function<void(int,int,int,int,int)>upd{[&upd](int l,int r,int i,int pos,int val){
		if(r-l==1)
			tr[i]=max(tr[i],val);
		else
		{
			int mid=(l+r)>>1;
			if(pos<mid)
				upd(l,mid,ls(i),pos,val);
			else
				upd(mid,r,rs(i),pos,val);
			tr[i]=max(tr[ls(i)],tr[rs(i)]);
		}
	}};

	// 区间 [L, R) 最大值查询
	function<int(int,int,int,int,int)>qry{[&qry](int l,int r,int i,int L,int R)->int{
		if(R<=l||r<=L)
			return -inf;
		if(L<=l&&r<=R)
			return tr[i];
		int mid=(l+r)>>1;
		return max(qry(l,mid,ls(i),L,R),qry(mid,r,rs(i),L,R));
	}};

	for(int i=0;i<m;++i)
	{
		char op;
		int x,y;
		cin>>op>>x>>y;
		if(op=='Q')
			cout<<qry(0,n,0,x-1,y)<<'\n';
		else
			upd(0,n,0,x-1,y);
	}
	return 0;
}
