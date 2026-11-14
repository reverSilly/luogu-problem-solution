#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 正解：混合背包（甲泛化 / 乙多重二进制拆分 / 丙完全）
i64 solve(int n,int m,vector<array<i64,4>>&it)
{
	vector<i64>f(m+1,0),v(m+1,0);
	for(auto&I:it)
	{
		i64 x=I[0],A=I[1],B=I[2],C=I[3];
		if(x==1)
		{
			for(int t=0;t<=m;++t)v[t]=A*t*t-B*t;
			for(int j=m;j>=0;--j)
				for(int t=1;t<=j;++t)
					f[j]=max(f[j],f[j-t]+v[t]);
		}
		else if(x==2)
		{
			for(i64 k=1;C>0;k<<=1)
			{
				i64 t=min(k,C);C-=t;
				i64 vol=B*t,val=A*t;
				for(int j=m;j>=vol;--j)
					f[j]=max(f[j],f[j-vol]+val);
			}
		}
		else
		{
			for(int j=B;j<=m;++j)
				f[j]=max(f[j],f[j-B]+A);
		}
	}
	return f[m];
}

// 暴力：递归枚举每件物品选取的体积/个数
int N,M;vector<array<i64,4>>IT;i64 best;
void dfs(int idx,int rem,i64 val)
{
	if(idx==N){best=max(best,val);return;}
	auto&I=IT[idx];i64 x=I[0],A=I[1],B=I[2],C=I[3];
	if(x==1)
		for(int t=0;t<=rem;++t)dfs(idx+1,rem-t,val+A*t*t-B*t);
	else if(x==2)
		for(i64 c=0;c<=C&&c*B<=rem;++c)dfs(idx+1,rem-(int)(c*B),val+c*A);
	else
		for(i64 c=0;c*B<=rem;++c)dfs(idx+1,rem-(int)(c*B),val+c*A);
}
i64 brute(int n,int m,vector<array<i64,4>>&it)
{
	N=n;M=m;IT=it;best=0;dfs(0,m,0);return best;
}

int main()
{
	mt19937 rn(12345);
	int T=3000,mis=0;
	for(int tc=0;tc<T;++tc)
	{
		int n=1+rn()%5,m=1+rn()%8;
		vector<array<i64,4>>it;
		for(int i=0;i<n;++i)
		{
			i64 x=1+rn()%3,A=rn()%7,B=rn()%7,C=rn()%7;
			if(x==3)B=1+rn()%6;	// 丙类保证 B>0
			it.push_back({x,A,B,C});
		}
		i64 s=solve(n,m,it),b=brute(n,m,it);
		if(s!=b)
		{
			++mis;
			if(mis<=5)
			{
				cerr<<"MISMATCH n="<<n<<" m="<<m<<" solve="<<s<<" brute="<<b<<"\n";
				for(auto&I:it)cerr<<I[0]<<" "<<I[1]<<" "<<I[2]<<" "<<I[3]<<"\n";
			}
		}
	}
	cout<<"tests="<<T<<" mismatches="<<mis<<"\n";
	return 0;
}