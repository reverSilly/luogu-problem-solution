// 题目：P2908 [USACO08OPEN] Word Power S
// 能量=名字中作为子序列出现的能量串个数（忽略大小写）
// 每个名字建 nxt[p][c]=位置≥p 的第一个字母 c（不存在=L），贪心最左匹配每个模式 O(|pat|)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxm{105},mxl{1005};

int n,m,nxt[mxl][26];
string s[mxn],t[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>s[i];
	for(int j=0;j<m;++j)
	{
		cin>>t[j];
		for(char&c:t[j])
			if(c>='A'&&c<='Z')
				c+=32;	// 大小写等价：统一折成小写
	}
	for(int i=0;i<n;++i)
	{
		int L=s[i].size();
		for(char&c:s[i])
			if(c>='A'&&c<='Z')
				c+=32;
		for(int c=0;c<26;++c)
			nxt[L][c]=L;	// 哨兵 L：后面再也没有字母 c
		for(int p=L-1;p>=0;--p)
		{
			for(int c=0;c<26;++c)
				nxt[p][c]=nxt[p+1][c];
			nxt[p][s[i][p]-'a']=p;
		}
		int ans=0;
		for(int j=0;j<m;++j)
		{
			int cur=0;
			bool ok=true;
			for(char c:t[j])
			{
				cur=nxt[cur][c-'a'];	// 贪心：取最靠左的可行匹配位
				if(cur==L){ok=false;break;}
				++cur;
			}
			ans+=ok;
		}
		cout<<ans<<'\n';
	}
	return 0;
}
