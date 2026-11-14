#include<bits/stdc++.h>
using namespace std;
// 临时对拍：埃氏筛 vs 试除法，核对素数对枚举（含奇数 k、empty）
mt19937 rn(7);
bool isprime(int x){if(x<2)return false;for(int d=2;d*d<=x;++d)if(x%d==0)return false;return true;}
int main()
{
	for(int it=0;it<20000;++it)
	{
		int n=1+rn()%200,k=1+rn()%n;
		// fast: sieve
		vector<bool>np(n+1,false);np[0]=true;if(n>=1)np[1]=true;
		for(int i=2;i*i<=n;++i)if(!np[i])for(int j=i*i;j<=n;j+=i)np[j]=true;
		vector<pair<int,int>>f,b;
		for(int p=2;p+k<=n;++p)if(!np[p]&&!np[p+k])f.push_back({p,p+k});
		for(int p=2;p+k<=n;++p)if(isprime(p)&&isprime(p+k))b.push_back({p,p+k});
		if(f!=b){printf("MISMATCH n=%d k=%d\n",n,k);return 1;}
	}
	puts("P2640 stress OK: 20000 cases, 0 mismatch");
	return 0;
}