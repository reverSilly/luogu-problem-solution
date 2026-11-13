// 题目：P2367 语文成绩
// 思路：区间加 -> 差分数组上两个单点修改；所有操作做完只做一次前缀和，边还原边取最小
//       n 到 5e6、操作到 5e6，输入量约 2e7 个整数，用 fread 自己读
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5000005};

int n,p,d[mxn];

char buf[1<<20];          // 1 MB 缓冲，把 fread 的调用次数压到几百次
int len=0,pos=0;

int gc()
{
	if(pos==len)
	{
		len=(int)fread(buf,1,sizeof(buf),stdin);
		pos=0;
		if(len<=0)
			return -1;
	}
	return buf[pos++];
}
int rd()
{
	int c=gc();
	while(c<'0'||c>'9')
		c=gc();
	int x=0;
	while(c>='0'&&c<='9')
	{
		x=x*10+(c-'0');
		c=gc();
	}
	return x;
}

int main()
{
	n=rd();p=rd();
	for(int i=1;i<=n;++i)
		d[i]=rd();                 // 先存原数组
	for(int i=n;i>=1;--i)
		d[i]-=d[i-1];              // 倒序原地变成差分：d[i]=a[i]-a[i-1]，d[0]=0
	for(int i=0;i<p;++i)
	{
		int x=rd(),y=rd(),z=rd();
		d[x]+=z;
		d[y+1]-=z;                 // y=n 时写的是幽灵位 d[n+1]，不影响任何 a[i]
	}
	int ans=1145141919,cur=0;
	for(int i=1;i<=n;++i)
	{
		cur+=d[i];                 // 前缀和还原 A[i]
		if(cur<ans)
			ans=cur;
	}
	printf("%d\n",ans);
	return 0;
}
