//
// 题目：P1655 小朋友的球
// https://www.luogu.com.cn/problem/P1655
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

struct bignum
{
	vector<int>d;
	bignum(int v=0)
	{
		if(v==0)
			d={0};
		else
			for(;v;v/=10)
				d.push_back(v%10);
	}
};

bignum add(const bignum&a,const bignum&b)
{
	bignum c;
	c.d.clear();
	int n=max(a.d.size(),b.d.size()),carry=0;
	for(int i=0;i<n||carry;++i)
	{
		int s=carry+(i<(int)a.d.size()?a.d[i]:0)+(i<(int)b.d.size()?b.d[i]:0);
		c.d.push_back(s%10);
		carry=s/10;
	}
	return c;
}

bignum mul(const bignum&a,int k)
{
	if(k==0||(a.d.size()==1&&a.d[0]==0))
		return bignum(0);
	bignum c;
	c.d.clear();
	int carry=0;
	for(int i=0;i<(int)a.d.size()||carry;++i)
	{
		int s=carry+(i<(int)a.d.size()?a.d[i]*k:0);
		c.d.push_back(s%10);
		carry=s/10;
	}
	return c;
}

bignum S[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	S[0][0]=bignum(1);
	for(int i=1;i<=100;++i)
		for(int j=1;j<=i;++j)
			S[i][j]=add(S[i-1][j-1],mul(S[i-1][j],j));

	int n,m;
	while(cin>>n>>m)
	{
		if(m<=0||n<m)
		{
			cout<<0<<'\n';
			continue;
		}
		const auto&v=S[n][m].d;
		for(int i=(int)v.size()-1;i>=0;--i)
			cout<<v[i];
		cout<<'\n';
	}
	return 0;
}
