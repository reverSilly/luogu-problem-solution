//
// 题目：P1587 [NOI2016] 循环之美
// https://www.luogu.com.cn/problem/P1587
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxb{2000005};       // 线性筛预处理上界 2*10^6
constexpr int mxk{2005};

int n,m,k;
int primes[mxb],pcnt;
int mu[mxb],pref_mu[mxb];
bool not_p[mxb];
int cop_pref[mxk];                // cop_pref[i]: 1..i 中与 k 互质的数的个数

unordered_map<long long,long long> memo_s;

void init_sieve()
{
	mu[1]=1;
	for(int i=2;i<mxb;++i)
	{
		if(!not_p[i])
		{
			primes[pcnt++]=i;
			mu[i]=-1;
		}
		for(int j=0;j<pcnt&&1LL*i*primes[j]<mxb;++j)
		{
			int v=i*primes[j];
			not_p[v]=true;
			if(i%primes[j]==0)
			{
				mu[v]=0;
				break;
			}
			mu[v]=-mu[i];
		}
	}
	for(int i=1;i<mxb;++i)
		pref_mu[i]=pref_mu[i-1]+mu[i];
}

// O(1) 求 sum_{y=1}^x [gcd(y, k) == 1]
long long calc_g(int x)
{
	if(x<=0)
		return 0;
	return 1LL*(x/k)*cop_pref[k]+cop_pref[x%k];
}

// 递归 + 杜教筛计算 S(x, c) = sum_{d=1}^x mu(d) [gcd(d, c) == 1], 其中 c 为无平方因子数
long long calc_S(int x,int c)
{
	if(x<=0)
		return 0;
	if(c==1&&x<mxb)
		return pref_mu[x];
	long long key=(1LL*x<<12)|c;
	auto it=memo_s.find(key);
	if(it!=memo_s.end())
		return it->second;

	long long res=0;
	if(c==1)
	{
		res=1;
		for(int l=2,r;l<=x;l=r+1)
		{
			int v=x/l;
			r=x/v;
			res-=1LL*(r-l+1)*calc_S(v,1);
		}
	}
	else
	{
		// 取 c 的最小质因子 p, 利用 S(x, c) = S(x, c/p) + S(x/p, c)
		int p=c;
		for(int i=2;i*i<=c;++i)
			if(c%i==0)
			{
				p=i;
				break;
			}
		res=calc_S(x,c/p)+calc_S(x/p,c);
	}
	return memo_s[key]=res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m>>k))
		return 0;
	init_sieve();

	for(int i=1;i<=k;++i)
		cop_pref[i]=cop_pref[i-1]+(std::gcd(i,k)==1);

	// 提取 k 的无平方因子核 rad_k
	int rad_k=1,tmp=k;
	for(int i=2;i*i<=tmp;++i)
		if(tmp%i==0)
		{
			rad_k*=i;
			while(tmp%i==0)
				tmp/=i;
		}
	if(tmp>1)
		rad_k*=tmp;

	memo_s.reserve(1<<16);
	int lim=min(n,m);
	long long ans=0,last_s=0;
	for(int l=1,r;l<=lim;l=r+1)
	{
		int vn=n/l,vm=m/l;
		r=min(n/vn,m/vm);
		long long cur_s=calc_S(r,rad_k);
		ans+=1LL*(cur_s-last_s)*vn*calc_g(vm);
		last_s=cur_s;
	}

	cout<<ans<<'\n';
	return 0;
}
