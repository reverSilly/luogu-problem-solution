// 题目：P2786 英语1（eng1）- 英语作文
// 词汇表塞进哈希表（同词多行则含金量累加），作文逐字符扫描，
// 非字母数字字符一律当分隔符，切出的极大字母数字段就是"单词"，整词查表累加，最后模 P。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n;
i64 p,ans;
unordered_map<string,i64>mp;

bool isword(char c)
{
	return (c>='0'&&c<='9')||(c>='a'&&c<='z')||(c>='A'&&c<='Z');
}
void add(const string&tok)
{
	auto it=mp.find(tok);
	if(it!=mp.end())
		ans=(ans+it->second)%p;	// ans<p<=1e9，词值<=1e10，i64 不溢出
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>p;
	mp.reserve(n*2);
	for(int i=0,b;i<n;++i)
	{
		string a;
		cin>>a>>b;
		mp[a]+=b;
	}
	string tok;
	for(char c;cin.get(c);)
	{
		if(isword(c))
			tok+=c;
		else if(!tok.empty())
		{
			add(tok);
			tok.clear();
		}
	}
	if(!tok.empty())
		add(tok);	// 作文末尾没有分隔符时最后一个单词在这里结算
	cout<<ans<<'\n';
	return 0;
}
