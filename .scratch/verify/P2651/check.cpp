#include<bits/stdc++.h>
using namespace std;
using i64=long long;
// 在进程内对拍：gcd 判据 vs 枚举所有加括号方案（精确分数判整）
int n;i64 a[10];
struct frac{i64 num,den;};
vector<frac> solve(int l,int r)
{
	if(l==r)return {{a[l],1}};
	vector<frac>res;
	for(int k=l;k<r;++k)
	{
		auto L=solve(l,k),R=solve(k+1,r);
		for(auto x:L)for(auto y:R)
			res.push_back({x.num*y.den,x.den*y.num}); // x/y
	}
	return res;
}
bool brute()
{
	auto all=solve(0,n-1);
	for(auto f:all)
		if(f.num%f.den==0)
			return true;
	return false;
}
bool criterion()
{
	i64 rem=a[1];
	for(int i=0;i<n&&rem>1;++i)
	{
		if(i==1)continue;
		rem/=gcd(rem,a[i]);
	}
	return rem==1;
}
int main()
{
	mt19937 rn(2024);
	int mismatch=0,yes=0,no=0;
	for(int t=0;t<30000;++t)
	{
		n=2+rn()%5; // 2..6
		for(int i=0;i<n;++i)a[i]=1+rn()%25;
		bool b=brute(),c=criterion();
		if(b!=c)
		{
			++mismatch;
			cout<<"MISMATCH n="<<n<<" brute="<<b<<" criterion="<<c<<" a:";
			for(int i=0;i<n;++i)cout<<' '<<a[i];
			cout<<'\n';
			if(mismatch>5)break;
		}
		if(c)++yes;else++no;
	}
	cout<<"done 30000 cases, mismatch="<<mismatch<<" yes="<<yes<<" no="<<no<<'\n';
	return 0;
}