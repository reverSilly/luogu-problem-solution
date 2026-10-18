//
// 题目：P1527 [国家集训队] 矩阵乘法
// https://www.luogu.com.cn/problem/P1527
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};           // n<=500 (+5 松弛)
constexpr int mxq{60005};         // q<=60000 (+5 松弛)

int n,q;
int tr[mxn][mxn];                 // 二维树状数组
int Ans[mxq];

struct Cell
{
	int r,c,v;
	friend bool operator<(Cell a,Cell b){return a.v<b.v;}
}cells[mxn*mxn];

struct Query
{
	int x1,y1,x2,y2,k,id;
}Q[mxq],ql_buf[mxq],qr_buf[mxq];

int lowbit(int x){return x&-x;}

void add(int r,int c,int v)
{
	for(int i=r+1;i<=n;i+=lowbit(i))
		for(int j=c+1;j<=n;j+=lowbit(j))
			tr[i][j]+=v;
}

int sigma(int r,int c)            // 前缀矩形 [0..r, 0..c] 计数 (r<0 或 c<0 时返回 0)
{
	int res=0;
	for(int i=r+1;i>0;i-=lowbit(i))
		for(int j=c+1;j>0;j-=lowbit(j))
			res+=tr[i][j];
	return res;
}

int rectSum(int x1,int y1,int x2,int y2)
{
	return sigma(x2,y2)-sigma(x1-1,y2)-sigma(x2,y1-1)+sigma(x1-1,y1-1);
}

// 整体二分: 单元下标半开区间 [cl, cr), 询问下标半开区间 [ql, qr)
void solve(int cl,int cr,int ql,int qr)
{
	if(ql>=qr)
		return;
	if(cr-cl==1)
	{
		for(int i=ql;i<qr;++i)
			Ans[Q[i].id]=cells[cl].v;
		return;
	}
	int mid=(cl+cr)>>1;
	for(int i=cl;i<mid;++i)
		add(cells[i].r,cells[i].c,1);

	int nl=0,nr=0;
	for(int i=ql;i<qr;++i)
	{
		int cnt=rectSum(Q[i].x1,Q[i].y1,Q[i].x2,Q[i].y2);
		if(Q[i].k<=cnt)
			ql_buf[nl++]=Q[i];
		else
		{
			Q[i].k-=cnt;
			qr_buf[nr++]=Q[i];
		}
	}
	for(int i=cl;i<mid;++i)
		add(cells[i].r,cells[i].c,-1);

	for(int i=0;i<nl;++i)
		Q[ql+i]=ql_buf[i];
	for(int i=0;i<nr;++i)
		Q[ql+nl+i]=qr_buf[i];

	solve(cl,mid,ql,ql+nl);
	solve(mid,cr,ql+nl,qr);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>q))
		return 0;
	int tot=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			int v;
			cin>>v;
			cells[tot++]={i,j,v};
		}
	sort(cells,cells+tot);

	for(int i=0;i<q;++i)
	{
		int x1,y1,x2,y2,k;
		cin>>x1>>y1>>x2>>y2>>k;
		--x1;--y1;--x2;--y2;
		Q[i]={x1,y1,x2,y2,k,i};
	}

	solve(0,tot,0,q);
	for(int i=0;i<q;++i)
		cout<<Ans[i]<<'\n';
	return 0;
}
