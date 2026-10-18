//
// 题目：P1533 可怜的狗狗
// https://www.luogu.com.cn/problem/P1533
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};        // n<=300000 (+5 松弛)
constexpr int mxm{50005};         // m<=50000 (+5 松弛)

int n,m;
int a[mxn],vals[mxn],vcnt;
int bit[mxn];                     // 树状数组维护当前滑动窗口内各离散化权值的出现次数
int Ans[mxm];

struct Query
{
	int l,r,k,id;
	friend bool operator<(Query x,Query y){return x.l!=y.l?x.l<y.l:x.r<y.r;}
}Q[mxm];

int lowbit(int x){return x&-x;}

void add(int idx,int v)
{
	for(int i=idx+1;i<=vcnt;i+=lowbit(i))
		bit[i]+=v;
}

// 树状数组倍增(Binary Lifting) O(log n) 求第 k 小离散化下标(0-based)
int kth(int k)
{
	int pos=0,sum=0;
	for(int step=1<<18;step>0;step>>=1)
		if(pos+step<=vcnt&&sum+bit[pos+step]<k)
		{
			pos+=step;
			sum+=bit[pos];
		}
	return pos;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		vals[i]=a[i];
	}
	sort(vals,vals+n);
	vcnt=unique(vals,vals+n)-vals;
	for(int i=0;i<n;++i)
		a[i]=lower_bound(vals,vals+vcnt,a[i])-vals;

	for(int i=0;i<m;++i)
	{
		cin>>Q[i].l>>Q[i].r>>Q[i].k;
		--Q[i].l;--Q[i].r;
		Q[i].id=i;
	}
	// 题面关键性质: 区间互不包含 => 按左端点升序后, 右端点也单调不降!
	sort(Q,Q+m);

	int L=0,R=-1;
	for(int i=0;i<m;++i)
	{
		while(R<Q[i].r)
			add(a[++R],1);
		while(L<Q[i].l)
			add(a[L++],-1);
		while(L>Q[i].l)
			add(a[--L],1);
		while(R>Q[i].r)
			add(a[R--],-1);
		Ans[Q[i].id]=vals[kth(Q[i].k)];
	}

	for(int i=0;i<m;++i)
		cout<<Ans[i]<<'\n';
	return 0;
}
