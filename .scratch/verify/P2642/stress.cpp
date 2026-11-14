#include<bits/stdc++.h>
using namespace std;
using i64=long long;
// 临时对拍：O(n^4) 暴力枚举两段 vs O(n) 前后缀 Kadane
mt19937 rn(12345);
int n;
i64 a[64],lf[64],rt[64];
i64 fast()
{
	i64 cur=a[0];lf[0]=a[0];
	for(int i=1;i<n;++i){cur=max(a[i],cur+a[i]);lf[i]=max(lf[i-1],cur);}
	cur=a[n-1];rt[n-1]=a[n-1];
	for(int i=n-2;i>=0;--i){cur=max(a[i],cur+a[i]);rt[i]=max(rt[i+1],cur);}
	i64 ans=LLONG_MIN;
	for(int g=1;g+1<n;++g)ans=max(ans,lf[g-1]+rt[g+1]);
	return ans;
}
i64 brute()
{
	i64 ans=LLONG_MIN;
	for(int l1=0;l1<n;++l1)for(int r1=l1;r1<n;++r1)
		for(int l2=r1+2;l2<n;++l2)for(int r2=l2;r2<n;++r2)
		{
			i64 s=0;
			for(int t=l1;t<=r1;++t)s+=a[t];
			for(int t=l2;t<=r2;++t)s+=a[t];
			ans=max(ans,s);
		}
	return ans;
}
int main()
{
	for(int it=0;it<200000;++it)
	{
		n=3+rn()%8;
		for(int i=0;i<n;++i)a[i]=(i64)(rn()%19)-9;
		i64 f=fast(),b=brute();
		if(f!=b){printf("MISMATCH it=%d n=%d fast=%lld brute=%lld\n",it,n,f,b);for(int i=0;i<n;++i)printf("%lld ",a[i]);puts("");return 1;}
	}
	puts("P2642 stress OK: 200000 cases, 0 mismatch");
	return 0;
}