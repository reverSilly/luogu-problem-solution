// 题目：P3090 [USACO13NOV] Empty Stalls G
// 每头牛占用「从喜好房间起向前第一个空房」⇒ successor 并查集：f[i]=从 i 出发第一个空房候选
// 占用 r 后 f[r]=(r+1)%n；全程 f[i]==i ⟺ i 空。答案=最小的 f[i]==i。
// 迭代式路径压缩：链长可达 3e6，递归会爆栈。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{3000005};

int n,k,f[mxn];

int getrt(int x)
{
	int r=x;
	while(f[r]!=r)
		r=f[r];
	while(f[x]!=x)
	{
		int t=f[x];
		f[x]=r;
		x=t;
	}
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	iota(f,f+n,0);
	for(int i=0,X,Y;i<k;++i)
	{
		i64 A,B;
		cin>>X>>Y>>A>>B;
		for(int j=1;j<=Y;++j)
		{
			int p=(A*j+B)%n;	// A*j 最大 3e15，必须 i64
			for(int t=0;t<X;++t)
			{
				int r=getrt(p);
				f[r]=(r+1)%n;	// r 被占用，指向环上下一间
			}
		}
	}
	for(int i=0;i<n;++i)
		if(f[i]==i)
		{
			cout<<i<<'\n';
			return 0;
		}
	return 0;
}
