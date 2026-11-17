// 题目：P3026 [USACO11OPEN] Learning Languages S
// 答案 = 牛-语言图的连通块数 - 1（K_i>=1 保证无"全文盲"特判）
// 每门语言记首个使用者 fst[l]，把该语言的所有使用者星形合并，等价于两两连边
// SPJ 缺失：只输出第一行的最少书数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005},mxm{30005};

int n,m,f[mxn],fst[mxm];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	iota(f,f+n,0);
	fill_n(fst,m,-1);	// 0 是合法牛下标，"尚无牛会"必须用 -1，不能靠默认 0
	for(int i=0,k,l;i<n;++i)
	{
		cin>>k;
		for(int j=0;j<k;++j)
		{
			cin>>l;--l;
			if(fst[l]==-1)
				fst[l]=i;
			else
				f[getrt(i)]=getrt(fst[l]);	// 星形合并：都挂到首个使用者上
		}
	}
	int ans=0;
	for(int i=0;i<n;++i)
		if(getrt(i)==i)
			++ans;
	cout<<ans-1<<'\n';
	return 0;
}
