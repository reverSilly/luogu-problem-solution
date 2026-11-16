// 自检：P2773 公式 vs 穷举所有 O/X 串（a,b,p,q<=5，串长<=10）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	int cnt=0;
	for(int a=0;a<=5;++a)for(int b=0;b<=5;++b)for(int p=0;p<=5;++p)for(int q=0;q<=5;++q)
	{
		// 暴力：枚举长度 0..a+b 的所有串
		int best=0;
		for(int len=0;len<=a+b;++len)
			for(int mask=0;mask<(1<<len);++mask)
			{
				int co=0,cx=0,ro=0,rx=0,mo=0,mx=0;
				for(int i=0;i<len;++i)
					if(mask>>i&1){++co;++ro;rx=0;mo=max(mo,ro);}
					else{++cx;++rx;ro=0;mx=max(mx,rx);}
				if(co<=a&&cx<=b&&mo<=p&&mx<=q)
					best=max(best,len);
			}
		// 公式
		i64 A=a,B=b,P=p,Q=q;
		if(P==0)A=0;
		if(Q==0)B=0;
		i64 ans=min(A,(B+1)*P)+min(B,(A+1)*Q);
		if(ans!=best)
		{
			printf("MISMATCH a=%d b=%d p=%d q=%d brute=%d formula=%lld\n",a,b,p,q,best,ans);
			return 1;
		}
		++cnt;
	}
	printf("OK %d cases\n",cnt);
	return 0;
}