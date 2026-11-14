#include<bits/stdc++.h>
using namespace std;

// 正解：预处理同字符连续段长度 run[]，每次掷骰 O(1)
pair<int,int> sol(const string&s,vector<int>&dice)
{
	int L=s.size();vector<int>run(L);
	run[L-1]=1;
	for(int i=L-2;i>=0;--i)run[i]=(s[i]==s[i+1])?run[i+1]+1:1;
	int pos=-1;
	for(int d:dice)
	{
		pos+=d;if(pos>=L)pos=L-1;
		if(s[pos]=='>'){int k=run[pos];if(k>=3)pos=min(pos+k,L-1);}
		else if(s[pos]=='*'){int k=run[pos];if(k>=3)pos=max(pos-k,0);}
	}
	return {pos+1,L-1-pos};
}

// 暴力：每次掷骰后现扫连续段（规则相同，实现独立）
pair<int,int> naive(const string&s,vector<int>&dice)
{
	int L=s.size();int pos=-1;
	for(int d:dice)
	{
		pos+=d;if(pos>=L)pos=L-1;
		char c=s[pos];
		if(c=='>'||c=='*')
		{
			int k=0;while(pos+k<L&&s[pos+k]==c)++k;
			if(k>=3)
			{
				if(c=='>')pos=min(pos+k,L-1);
				else pos=max(pos-k,0);
			}
		}
	}
	return {pos+1,L-1-pos};
}

int main()
{
	mt19937 rn(7);
	int T=20000,mis=0;
	string chars=">>**ab f";	// 含空格
	for(int tc=0;tc<T;++tc)
	{
		int L=1+rn()%14;string s;
		for(int i=0;i<L;++i)s+=chars[rn()%chars.size()];
		int n=1+rn()%7;vector<int>dice(n);
		for(int i=0;i<n;++i)dice[i]=1+rn()%6;
		auto a=sol(s,dice),b=naive(s,dice);
		if(a!=b){++mis;if(mis<=5)cerr<<"MISMATCH s=["<<s<<"]\n";}
	}
	cout<<"tests="<<T<<" mismatches="<<mis<<"\n";
	return 0;
}