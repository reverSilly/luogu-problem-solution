// 题目：P2659 美丽的序列
// 思路：美丽度=长*最小值 ⇒ 枚举"谁当最小值"，答案=max_i a[i]*(左右第一个严格更小者之间的跨度)。
//       这就是直方图最大矩形，单调栈 O(n)。
//       严格 > 弹栈：相等段最左位置拿到完整跨度；哨兵 a[n]=-1 逼空栈。
//       乘积<=4e12 用 i64；n=2e6 用 fread 快读（题面提示需要读入优化）。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};

int n,a[mxn],st[mxn],tp;
i64 ans;

// fread 快读：n 达 2e6，cin 可能吃紧
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
		while(tp&&a[st[tp-1]]>a[i])	// 严格 > 才弹，相等留在栈里
		{
			int j=st[--tp];
			int l=tp?st[tp-1]:-1;	// 左边第一个 <=a[j] 的仍在栈者
			ans=max(ans,(i64)a[j]*(i-l-1));	// 高 a[j]，宽 i-l-1
		}
		st[tp++]=i;
	}
	printf("%lld\n",ans);
	return 0;
}
