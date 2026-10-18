//
// 题目：P1520 因式分解
// 暴力基准: 递归定义 Phi_d(x) = (x^d - 1) / \prod_{k|d, k<d} Phi_k(x), 直接用多项式长除法求解
#include<bits/stdc++.h>
using namespace std;

struct Poly
{
	int deg;
	vector<int>a;
};

// 首一多项式整除 A / B
Poly divPoly(Poly A,const Poly&B)
{
	int da=A.deg,db=B.deg;
	vector<int>q(da-db+1,0);
	for(int i=da;i>=db;--i)
	{
		int c=A.a[i];
		q[i-db]=c;
		if(c!=0)
			for(int j=0;j<=db;++j)
				A.a[i-db+j]-=c*B.a[j];
	}
	return {da-db,move(q)};
}

bool cmpPoly(const Poly&A,const Poly&B)
{
	if(A.deg!=B.deg)
		return A.deg<B.deg;
	for(int i=A.deg;i>=0;--i)
	{
		if(abs(A.a[i])!=abs(B.a[i]))
			return abs(A.a[i])<abs(B.a[i]);
		if(A.a[i]!=B.a[i])
			return A.a[i]<B.a[i];
	}
	return false;
}

void printPoly(const Poly&P)
{
	for(int i=P.deg;i>=0;--i)
	{
		int c=P.a[i];
		if(c==0)
			continue;
		if(i<P.deg&&c>0)
			cout<<'+';
		if(i==0)
			cout<<c;
		else
		{
			if(c==-1)
				cout<<'-';
			else if(c!=1)
				cout<<c;
			cout<<'x';
			if(i>1)
				cout<<'^'<<i;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	if(!(cin>>n))
		return 0;

	vector<int>divs;
	for(int d=1;d*d<=n;++d)
		if(n%d==0)
		{
			divs.push_back(d);
			if(d*d<n)
				divs.push_back(n/d);
		}
	sort(divs.begin(),divs.end());

	int m=(int)divs.size();
	vector<Poly>phi(m);
	for(int i=0;i<m;++i)
	{
		int d=divs[i];
		vector<int>cur(d+1,0);
		cur[d]=1;cur[0]=-1;
		Poly P{d,move(cur)};
		for(int j=0;j<i;++j)
			if(d%divs[j]==0)
				P=divPoly(move(P),phi[j]);
		phi[i]=move(P);
	}

	sort(phi.begin(),phi.end(),cmpPoly);
	if(phi.size()==1)
		printPoly(phi[0]);
	else
		for(const auto&P:phi)
		{
			cout<<'(';
			printPoly(P);
			cout<<')';
		}
	cout<<'\n';
	return 0;
}
