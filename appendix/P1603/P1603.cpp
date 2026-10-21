//
// 题目：P1603 斯诺登的密码
// https://www.luogu.com.cn/problem/P1603
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};          // 整句字符数 <=1000 (+5 松弛)

map<string,int> mp;               // 英文单词 -> 数值
string line,w;
vector<int> blk;                  // 每个数字平方模 100 后的两位块

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	const char* num[21]={"zero","one","two","three","four","five","six","seven","eight",
		"nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
		"seventeen","eighteen","nineteen","twenty"};
	for(int i=0;i<=20;++i)
		mp[num[i]]=i;
	mp["a"]=1;mp["another"]=1;mp["first"]=1;   // 非正规写法
	mp["both"]=2;mp["second"]=2;
	mp["third"]=3;

	getline(cin,line);
	for(size_t i=0;i<=line.size();++i)   // 按非字母字符切词, 顺带丢掉句号
	{
		char c=i<line.size()?line[i]:' ';
		if(isalpha((unsigned char)c))
			w+=char(tolower((unsigned char)c));
		else if(!w.empty())
		{
			auto it=mp.find(w);
			if(it!=mp.end())
				blk.push_back(it->second*it->second%100);
			w.clear();
		}
	}

	sort(blk.begin(),blk.end());      // 等长两位块, 升序拼接即为最小

	string ans;
	for(int v:blk)
	{
		ans+=char('0'+v/10);
		ans+=char('0'+v%10);
	}
	size_t p=0;                      // 去掉整个串的前导零
	while(p<ans.size()&&ans[p]=='0')
		++p;
	if(p==ans.size())
		cout<<0<<'\n';           // 全为 00 时答案为 0
	else
		cout<<ans.substr(p)<<'\n';
	return 0;
}
