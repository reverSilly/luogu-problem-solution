//
// 题目：P1520 因式分解
// https://www.luogu.com.cn/problem/P1520
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};          // n<=5000 (+5 松弛)

int n;
int mu[mxn],pr[mxn],pcnt;
bool vis[mxn];

struct Poly
{
	int deg;
	vector<int>a;                 // a[i] 为 x^i 的系数 (0<=i<=deg, 首项 a[deg]=1)
};

// 线性筛预处理莫比乌斯函数 mu[1..n]
void ini(int lim)
{
	mu[1]=1;
	for(int i=2;i<=lim;++i)
	{
		if(!vis[i])
			pr[pcnt++]=i,mu[i]=-1;
		for(int j=0;j<pcnt&&i*pr[j]<=lim;++j)
		{
			vis[i*pr[j]]=true;
			if(i%pr[j]==0)
			{
				mu[i*pr[j]]=0;
				break;
			}
			mu[i*pr[j]]=-mu[i];
		}
	}
}

// 用莫比乌斯反演公式 Phi_d(x) = \prod_{k|d} (x^k - 1)^{\mu(d/k)} 计算 d 次分圆多项式
Poly calcPhi(int d)
{
	vector<int>f{1};              // 先乘所有 mu(d/k)==1 的 (x^k - 1)
	for(int k=1;k<=d;++k)
	{
		if(d%k!=0||mu[d/k]!=1)
			continue;
		int old=(int)f.size();
		f.resize(old+k,0);
		for(int i=old+k-1;i>=k;--i)
			f[i]=f[i-k]-f[i];
		for(int i=0;i<k;++i)
			f[i]=-f[i];
	}
	// 再依次除以所有 mu(d/k)==-1 的 (x^k - 1)
	for(int k=1;k<=d;++k)
	{
		if(d%k!=0||mu[d/k]!=-1)
			continue;
		int deg=(int)f.size()-1;
		vector<int>q(deg-k+1,0);
		for(int i=deg;i>=k;--i)
		{
			q[i-k]=f[i];
			f[i-k]+=f[i];
		}
		f.swap(q);
	}
	return {(int)f.size()-1,move(f)};
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

	if(!(cin>>n))
		return 0;
	ini(n);

	vector<Poly>facs;
	for(int d=1;d*d<=n;++d)
	{
		if(n%d!=0)
			continue;
		facs.push_back(calcPhi(d));
		if(d*d<n)
			facs.push_back(calcPhi(n/d));
	}
	sort(facs.begin(),facs.end(),cmpPoly);

	if(facs.size()==1)
		printPoly(facs[0]);
	else
		for(const auto&P:facs)
		{
			cout<<'(';
			printPoly(P);
			cout<<')';
		}
	cout<<'\n';
	return 0;
}
