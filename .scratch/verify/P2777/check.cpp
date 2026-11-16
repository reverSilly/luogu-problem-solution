// 自检：P2777 前缀/后缀做法 vs 枚举所有得分排列（n<=6 随机 3000 组）
#include<bits/stdc++.h>
using namespace std;

constexpr int inf{1145141919};

int fast(int n,vector<int>b)
{
	sort(b.begin(),b.end());
	vector<int>f(n),pre(n+1),suf(n+1);
	for(int i=0;i<n;++i)f[i]=b[i]+n-1-i;
	pre[0]=-inf;
	for(int i=1;i<n;++i)pre[i]=max(pre[i-1],f[i-1]);
	suf[n]=-inf;
	for(int i=n-1;i>=0;--i)suf[i]=max(suf[i+1],f[i]);
	int ans=0;
	for(int i=0;i<n;++i)
		if(b[i]+n>max(pre[i],suf[i+1]+1))
			++ans;
	return ans;
}

int brute(int n,vector<int>b)
{
	vector<int>p(n);
	iota(p.begin(),p.end(),1);
	int ans=0;
	for(int i=0;i<n;++i)
	{
		bool ok=false;
		iota(p.begin(),p.end(),1);	// 每个候选人必须从首个排列重新枚举
		do
		{
			bool win=true;
			for(int j=0;j<n;++j)
				if(j!=i&&b[j]+p[j]>=b[i]+p[i]){win=false;break;}
			if(win){ok=true;break;}
		}while(next_permutation(p.begin(),p.end()));
		ans+=ok;
	}
	return ans;
}

int main()
{
	mt19937 rn(20260214);
	for(int t=0;t<3000;++t)
	{
		int n=3+rn()%4;			// 3..6
		vector<int>b(n);
		for(int&v:b)v=rn()%7;	// 0..6
		int x=fast(n,b),y=brute(n,b);
		if(x!=y)
		{
			printf("MISMATCH n=%d b=",n);
			for(int v:b)printf("%d ",v);
			printf("fast=%d brute=%d\n",x,y);
			return 1;
		}
	}
	// 并列边界专项：[0,1,2] n=3 应为 2（严格冠军口径）
	if(fast(3,{0,1,2})!=2){puts("tie case wrong");return 1;}
	puts("OK 3000 cases + tie case");
	return 0;
}