#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005},mxv{20005};

int n;
struct cow{int v,x;}a[mxn];

struct BIT
{
	i64 tr[mxv]{};
	void add(int p,i64 w)
	{
		for(int i=p;i<mxv;i+=i&-i)
			tr[i]+=w;
	}
	i64 sigma(int p)
	{
		i64 s=0;
		for(int i=p;i>0;i-=i&-i)
			s+=tr[i];
		return s;
	}
};

BIT cnt,sm;

i64 fast(int nn,const vector<int>&V,const vector<int>&X)
{
	n=nn;
	for(int i=0;i<n;++i)
		a[i]={V[i],X[i]};
	memset(cnt.tr,0,sizeof(cnt.tr));
	memset(sm.tr,0,sizeof(sm.tr));
	sort(a,a+n,[](auto p,auto q){return p.v<q.v;});
	i64 tot=0,ans=0;
	for(int i=0;i<n;++i)
	{
		int v=a[i].v,x=a[i].x;
		int cl=(int)cnt.sigma(x-1);
		i64 sl=sm.sigma(x-1);
		i64 dl=(i64)x*cl-sl;
		i64 dr=(tot-sl)-(i64)x*(i-cl);
		ans+=(i64)v*(dl+dr);
		cnt.add(x,1);
		sm.add(x,x);
		tot+=x;
	}
	return ans;
}

i64 brute(int n,const vector<int>&v,const vector<int>&x)
{
	i64 ans=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			ans+=(i64)max(v[i],v[j])*abs(x[i]-x[j]);
	return ans;
}

int main()
{
	mt19937 rn(20241022);
	for(int t=1;t<=2000;++t)
	{
		int n=rn()%14+1;
		set<int>sx;
		while((int)sx.size()<n)
			sx.insert(rn()%40+1);
		vector<int>x(sx.begin(),sx.end());
		shuffle(x.begin(),x.end(),rn);
		vector<int>v(n);
		for(int i=0;i<n;++i)
			v[i]=rn()%25+1;
		i64 r1=fast(n,v,x),r2=brute(n,v,x);
		if(r1!=r2)
		{
			cerr<<"Mismatch at test "<<t<<": fast="<<r1<<" brute="<<r2<<'\n';
			cerr<<n<<'\n';
			for(int i=0;i<n;++i)
				cerr<<v[i]<<' '<<x[i]<<'\n';
			return 1;
		}
	}
	cout<<"all 2000 tests passed\n";
	return 0;
}
