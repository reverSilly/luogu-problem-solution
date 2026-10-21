//
// 题目：P1643 完美数
// https://www.luogu.com.cn/problem/P1643
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxd{20015};

int d[mxd],len_d;

void trim()
{
	while(len_d>1&&d[len_d-1]==0)
		--len_d;
}

bool ge_9pow(int p)
{
	if(len_d>p+1)
		return true;
	if(len_d<p+1)
		return false;
	return d[p]>=9;
}

void sub_at(int p,int v)
{
	int i=p;
	d[i]-=v;
	while(d[i]<0)
	{
		d[i]+=10;
		--d[++i];
	}
	trim();
}

void solve()
{
	string X,N;
	cin>>X>>N;
	int L=X.size(),m=(L+1)/2;
	string P0=X.substr(0,m);
	for(int i=L-m-1;i>=0;--i)
		P0.push_back(X[i]);

	bool dec=(X!="0"&&P0>X);

	int max_l=max(m,(int)N.size())+5;
	fill_n(d,max_l,0);
	for(int i=0;i<m;++i)
		d[i]+=X[m-1-i]-'0';
	for(int i=0;i<(int)N.size();++i)
		d[i]+=N[N.size()-1-i]-'0';
	if(dec)
		--d[0];

	len_d=max_l;
	for(int i=0;i<len_d-1;++i)
	{
		if(d[i]<0)
		{
			d[i]+=10;
			--d[i+1];
		}
		else if(d[i]>=10)
		{
			d[i+1]+=d[i]/10;
			d[i]%=10;
		}
	}
	trim();

	if(len_d>m)
	{
		sub_at(m,1);
		++L;
		m=(L+1)/2;
		while(ge_9pow(m-1))
		{
			sub_at(m-1,9);
			++L;
			m=(L+1)/2;
		}
		while(len_d<m)
			d[len_d++]=0;
		++d[m-1];
	}

	string H(m,'0');
	for(int i=0;i<m;++i)
		H[m-1-i]=char('0'+d[i]);
	string ans=H;
	for(int i=L-m-1;i>=0;--i)
		ans.push_back(H[i]);
	cout<<ans<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
		solve();
	return 0;
}
