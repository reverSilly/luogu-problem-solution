//
// 题目：P1655 小朋友的球
// 暴力基准：利用容斥原理 S(n,m) = (1/m!) sum_{k=0}^m (-1)^(m-k) C(m,k) k^n 计算小规模精确值
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 qpow(i64 a,int b)
{
	i64 r=1;
	for(int i=0;i<b;++i)
		r*=a;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	while(cin>>n>>m)
	{
		if(m<=0||n<m)
		{
			cout<<0<<'\n';
			continue;
		}
		__int128 sum=0,comb=1;
		for(int k=0;k<=m;++k)
		{
			if(k>0)
				comb=comb*(m-k+1)/k;
			__int128 term=comb;
			for(int i=0;i<n;++i)
				term*=k;
			if((m-k)&1)
				sum-=term;
			else
				sum+=term;
		}
		for(int i=1;i<=m;++i)
			sum/=i;
		string s;
		if(sum==0)
			s="0";
		else
			for(;sum>0;sum/=10)
				s.push_back(char('0'+int(sum%10)));
		reverse(s.begin(),s.end());
		cout<<s<<'\n';
	}
	return 0;
}
