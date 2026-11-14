#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 正解：只枚举 p_i 作为候选价格
pair<int,i64> solve(int n,vector<int>p)
{
	int m=p.size();sort(p.begin(),p.end());
	i64 best=0;int bp=0;
	for(int i=0;i<m;++i)
	{
		int price=p[i];
		int cnt=m-(lower_bound(p.begin(),p.end(),price)-p.begin());
		i64 rev=(i64)price*min(n,cnt);
		if(rev>best||(rev==best&&price<bp))best=rev,bp=price;
	}
	return {bp,best};
}
// 暴力：枚举所有整数价格 1..maxp
pair<int,i64> brute(int n,vector<int>p)
{
	int mx=*max_element(p.begin(),p.end());
	i64 best=-1;int bp=0;
	for(int price=1;price<=mx;++price)
	{
		int cnt=0;for(int x:p)if(x>=price)++cnt;
		i64 rev=(i64)price*min(n,cnt);
		if(rev>best||(rev==best&&price<bp))best=rev,bp=price;
	}
	return {bp,best};
}
int main()
{
	mt19937 rn(7);int bad=0;
	for(int t=0;t<50000;++t)
	{
		int n=1+rn()%12,m=1+rn()%12;vector<int>p(m);
		for(int&x:p)x=1+rn()%30;
		auto a=solve(n,p),b=brute(n,p);
		if(a!=b){++bad;if(bad<=5){cerr<<"bad n="<<n<<" p:";for(int x:p)cerr<<x<<' ';cerr<<" got "<<a.first<<' '<<a.second<<" exp "<<b.first<<' '<<b.second<<"\n";}}
	}
	cerr<<"mismatch="<<bad<<"\n";
	return 0;
}