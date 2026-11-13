// 题目：P2369 EXCEEDED WARNING A
// 思路：只要最小的 m 个 => 用一个大小为 m 的「大根堆」当候选集
//       堆顶是候选集里的最大值：新数比它小就顶替它，否则直接丢弃
//       最后把堆数组排序输出。只用 O(m) 内存（Memory Limit = 4000KiB）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{100005};        // m<1e5，堆里只放 m 个数

int n,m,sz;
int h[mxm];                       // 大根堆：h[0] 是堆中最大的那个

char buf[1<<16];
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

void up(int i)                    // 新元素上浮
{
	while(i>0)
	{
		int f=(i-1)>>1;
		if(h[f]>=h[i])
			break;
		swap(h[f],h[i]);
		i=f;
	}
}
void down(int i)                  // 堆顶下沉
{
	for(;;)
	{
		int l=i*2+1,r=l+1,b=i;
		if(l<sz&&h[l]>h[b])b=l;
		if(r<sz&&h[r]>h[b])b=r;
		if(b==i)
			break;
		swap(h[i],h[b]);
		i=b;
	}
}

int main()
{
	n=rd();m=rd();
	for(int i=0,x;i<n;++i)
	{
		x=rd();
		if(sz<m)                  // 堆还没装满，直接塞
		{
			h[sz]=x;
			up(sz);
			++sz;
		}
		else if(x<h[0])           // 比堆中最大还小，替换掉它
		{
			h[0]=x;
			down(0);
		}
	}
	sort(h,h+sz);
	for(int i=0;i<sz&&i<m;++i)
		printf("%d\n",h[i]);
	return 0;
}
