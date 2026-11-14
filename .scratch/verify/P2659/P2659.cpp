#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};

int n,a[mxn],st[mxn],tp;
i64 ans;

// fread 快读：n 达 2e6，题面明确提示需要读入优化
char ibuf[1<<20];int ipos,ilen;
inline int gc()
{
	if(ipos==ilen)
	{
		ilen=fread(ibuf,1,sizeof ibuf,stdin);
		ipos=0;
		if(!ilen)
			return EOF;
	}
	return ibuf[ipos++];
}
inline int rd()
{
	int x=0,c=gc();
	while(c<'0'||c>'9')
		c=gc();
	while(c>='0'&&c<='9')
	{
		x=x*10+(c-'0');
		c=gc();
	}
	return x;
}

int main()
{
	n=rd();
	for(int i=0;i<n;++i)
		a[i]=rd();
	a[n]=-1;	// 哨兵：比所有 a_i(>=0) 都小，逼出栈中全部元素
	for(int i=0;i<=n;++i)
	{
		while(tp&&a[st[tp-1]]>a[i])
		{
			int j=st[--tp];
			int l=tp?st[tp-1]:-1;	// 左边第一个严格更小者
			ans=max(ans,(i64)a[j]*(i-l-1));
		}
		st[tp++]=i;
	}
	printf("%lld\n",ans);
	return 0;
}