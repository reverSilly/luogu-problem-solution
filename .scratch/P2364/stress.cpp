// 对拍: 三串 LCS(回溯出串) vs 枚举 a 的全部子序列; 并校验输出是公共子序列
#include<bits/stdc++.h>
using namespace std;

static mt19937 rng(2024);

int dp[15][15][15];
string a,b,c;

string solve()
{
	int n=a.size(),m=b.size(),l=c.size();
	memset(dp,0,sizeof(dp));
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
			for(int k=1;k<=l;++k)
				if(a[i-1]==b[j-1]&&a[i-1]==c[k-1])
					dp[i][j][k]=dp[i-1][j-1][k-1]+1;
				else
					dp[i][j][k]=max({dp[i-1][j][k],dp[i][j-1][k],dp[i][j][k-1]});
	string ans;
	int i=n,j=m,k=l;
	while(i&&j&&k)
	{
		if(a[i-1]==b[j-1]&&a[i-1]==c[k-1])
		{
			ans+=a[i-1];
			--i;--j;--k;
		}
		else if(dp[i][j][k]==dp[i-1][j][k])
			--i;
		else if(dp[i][j][k]==dp[i][j-1][k])
			--j;
		else
			--k;
	}
	reverse(ans.begin(),ans.end());
	return ans;
}

bool issub(const string&s,const string&t)
{
	int p=0;
	for(char ch:t)
		if(p<(int)s.size()&&s[p]==ch)
			++p;
	return p==(int)s.size();
}

string brute()
{
	int n=a.size();
	string best="";
	for(int S=0;S<(1<<n);++S)
	{
		string s;
		for(int i=0;i<n;++i)
			if(S>>i&1)
				s+=a[i];
		if((int)s.size()<(int)best.size())
			continue;
		if(issub(s,b)&&issub(s,c)&&(int)s.size()>(int)best.size())
			best=s;
	}
	return best;
}

int main()
{
	int bad=0,emptycnt=0;
	for(int tc=0;tc<500;++tc)
	{
		int la=1+rng()%10,lb=1+rng()%8,lc=1+rng()%8;
		int al=2+rng()%2;
		a="";b="";c="";
		for(int i=0;i<la;++i)a+=(char)('a'+rng()%al);
		for(int i=0;i<lb;++i)b+=(char)('a'+rng()%al);
		for(int i=0;i<lc;++i)c+=(char)('a'+rng()%al);
		string s1=solve(),s2=brute();
		if(s1.size()!=s2.size())
		{
			printf("LEN MISMATCH [%s][%s][%s] -> [%s] vs [%s]\n",a.c_str(),b.c_str(),c.c_str(),s1.c_str(),s2.c_str());
			++bad;
		}
		else if(!issub(s1,a)||!issub(s1,b)||!issub(s1,c))
		{
			printf("NOT SUBSEQ [%s][%s][%s] -> [%s]\n",a.c_str(),b.c_str(),c.c_str(),s1.c_str());
			++bad;
		}
		if(s2.empty())++emptycnt;
	}
	printf("done bad=%d empty_answers=%d\n",bad,emptycnt);
	return 0;
}
