// 题目：P2264 情书
// 把正文切成句号分隔的句子，再把每句切成极大字母段
// 每句用一个 bool 数组去重，统计句内出现的不同词表单词
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n;
string w[mxn],text,tok;
bool got[mxn];

int id(const string&t)
{
	for(int i=0;i<n;++i)
		if(w[i]==t)
			return i;
	return -1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>w[i];
		for(char&c:w[i])
			c=tolower((unsigned char)c);
	}
	string line;
	getline(cin,line);                 // 吃掉词表最后一行的换行
	while(getline(cin,line))
		text+=line+'\n';               // 换行也是非字母分隔符

	int ans=0;
	auto flush=[&]()
	{
		if(tok.empty())
			return;
		int k=id(tok);
		if(k>=0&&!got[k])              // 同一句同一个词只加一次
		{
			got[k]=1;
			++ans;
		}
		tok.clear();
	};
	for(char c:text)
	{
		if(isalpha((unsigned char)c))
			tok+=tolower((unsigned char)c);
		else
		{
			flush();
			if(c=='.')                 // 句号：进入下一句，标记清零
				fill_n(got,n,false);
		}
	}
	flush();                           // 最后一句可能没有句号
	cout<<ans<<'\n';
	return 0;
}
