//
// 题目：P1624 [ECNA 2005] 单词缩写
// https://www.luogu.com.cn/problem/P1624
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{155};

int n;
set<string>bad;
string ab;
i64 dp[mxn],cur[mxn];                    // dp[j]:前若干个有效单词用掉缩写前 j 个字母的方案数

string trim(const string&s)
{
	int l=0,r=(int)s.size()-1;
	while(l<=r&&(s[l]==' '||s[l]=='\t'||s[l]=='\r'))
		++l;
	while(r>=l&&(s[r]==' '||s[r]=='\t'||s[r]=='\r'))
		--r;
	if(l>r)
		return "";
	return s.substr(l,r-l+1);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		bad.insert(s);
	}
	string line;
	getline(cin,line);
	while(cin>>ab)
	{
		if(ab=="LASTCASE")
			break;
		string ph;
		getline(cin,line);
		ph=trim(line);
		while(ph.empty())
		{
			if(!getline(cin,line))
				break;
			ph=trim(line);
		}
		int L=ab.size();
		fill(dp,dp+L+1,0);
		dp[0]=1;
		istringstream is(ph);
		string w;
		while(is>>w)
		{
			string low;
			for(char c:w)
				low+=(char)tolower(c);
			if(bad.count(low))
				continue;                       // 无效单词整词跳过
			fill(cur,cur+L+1,0);
			for(char c:w)
			{
				char u=(char)toupper(c);
				for(int j=L;j>=1;--j)
					if(u==ab[j-1])
						cur[j]+=cur[j-1]+dp[j-1];   // 接在本词内 / 本词开头
			}
			for(int j=0;j<=L;++j)
				dp[j]=cur[j];                   // 每个有效单词至少要贡献一个字母
		}
		if(dp[L]>0)
			cout<<ab<<" can be formed in "<<dp[L]<<" ways\n";
		else
			cout<<ab<<" is not a valid abbreviation\n";
	}
	return 0;
}
