#include<bits/stdc++.h>
using namespace std;
// 开发期校验：闭式 vs 概率DP vs 穷举硬币序列模拟
long double closed(int n)
{
	int m=n-1;
	long double p=1;
	for(int i=1;i<=m;++i)p=p*(m+i)/i/4;
	return 1-p;
}
map<pair<int,int>,long double>memo;
long double f(int a,int b)
{
	if(a+b==2)return (a==2||b==2)?1.0L:0.0L;
	auto it=memo.find({a,b});
	if(it!=memo.end())return it->second;
	long double r;
	if(a==0)r=f(0,b-1);
	else if(b==0)r=f(a-1,0);
	else r=0.5L*f(a-1,b)+0.5L*f(a,b-1);
	return memo[{a,b}]=r;
}
long double simul(int n)
{
	int total=2*n;
	long long same=0,all=1LL<<total;
	for(long long mask=0;mask<all;++mask)
	{
		int a=n,b=n;
		char last=0,prev=0;
		for(int t=0;t<total;++t)
		{
			int coin=(mask>>t)&1;
			char tk;
			if(a>0&&b>0){if(coin==0){--a;tk='A';}else{--b;tk='B';}}
			else if(a>0){--a;tk='A';}
			else{--b;tk='B';}
			prev=last;last=tk;
		}
		if(last==prev)++same;
	}
	return (long double)same/all;
}
int main()
{
	cout<<fixed<<setprecision(6);
	for(int n=1;n<=8;++n)
	{
		long double c=closed(n),d=f(n,n),s=simul(n);
		cout<<"n="<<n<<" closed="<<c<<" dp="<<d<<" simul="<<s;
		if(fabsl(c-d)>1e-9||fabsl(c-s)>1e-9){cout<<"  MISMATCH\n";return 1;}
		cout<<"  OK\n";
	}
	for(int n=9;n<=60;++n)
	{
		long double c=closed(n),d=f(n,n);
		if(fabsl(c-d)>1e-9){cout<<"n="<<n<<" closed="<<c<<" dp="<<d<<" MISMATCH\n";return 1;}
	}
	cout<<"n=9..60 closed==dp OK\n";
	cout<<"n=1250 closed="<<closed(1250)<<"\n";
	return 0;
}