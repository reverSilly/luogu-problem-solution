// P2793 轻量自检：事件式快进贪心 vs 逐秒暴力模拟（仅小数据随机对拍，验证快进取整边界）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 ceildiv(i64 x,i64 y){return (x+y-1)/y;}

i64 fast(int n,i64 h,i64 k,const vector<i64>&a)
{
	i64 w=0,t=0;
	int i=0;
	while(i<n||w>0)
	{
		while(i<n&&w+a[i]<=h)
			w+=a[i++];
		if(i==n&&w==0)
			break;
		i64 d=i<n?ceildiv(w-(h-a[i]),k):ceildiv(w,k);
		t+=d;
		w=max(0ll,w-d*k);
	}
	return t;
}
i64 brute(int n,i64 h,i64 k,const vector<i64>&a)
{
	i64 w=0,t=0;
	int i=0;
	while(i<n||w>0)
	{
		while(i<n&&w+a[i]<=h)
			w+=a[i++];
		if(i==n&&w==0)
			break;
		++t;
		w=max(0ll,w-k);
	}
	return t;
}

int main()
{
	mt19937 rn(20261116);
	for(int cs=0;cs<200000;++cs)
	{
		int n=rn()%12+1;
		i64 h=rn()%20+1,k=rn()%7+1;
		vector<i64>a(n);
		for(auto&x:a)
			x=rn()%h+1;
		i64 x=fast(n,h,k,a),y=brute(n,h,k,a);
		if(x!=y)
		{
			printf("MISMATCH case %d: n=%d h=%lld k=%lld fast=%lld brute=%lld\na:",cs,n,h,k,x,y);
			for(auto v:a)
				printf(" %lld",v);
			puts("");
			return 1;
		}
	}
	puts("200000 random cases: all match");
	return 0;
}