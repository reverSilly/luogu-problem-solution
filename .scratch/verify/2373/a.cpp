// 题目：P2373 yyy2015c01 的 IDE 之 Call Stack
// 把每行规范化成 key = 小写函数名(参数类型1,参数类型2,...)
// 形参名、返回类型、空格全都丢掉；main 一律跳过；set 去重后输出大小
#include<bits/stdc++.h>
using namespace std;

set<string> S;

vector<string> tok(const string&t)
{
	vector<string>r;
	istringstream in(t);
	string w;
	while(in>>w)
		r.push_back(w);
	return r;
}
string low(string t)
{
	for(char&c:t)
		c=(char)tolower(c);
	return t;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	string line;
	getline(cin,line);            //吃掉第一行行尾
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		while(!line.empty()&&(line.back()=='\r'||line.back()==' '||line.back()=='\t'))
			line.pop_back();
		size_t lp=line.find('(');
		size_t rp=lp==string::npos?string::npos:line.find(')',lp);
		string head=lp==string::npos?line:line.substr(0,lp);
		string body=(lp==string::npos||rp==string::npos)?"":line.substr(lp+1,rp-lp-1);
		auto tk=tok(head);
		if(tk.empty())
			continue;               //空行，不是调用
		string name=low(tk.back()); //返回类型丢弃，函数名取 '(' 前最后一个 token
		if(name=="main")
			continue;               //main 不计入
		string key=name+'(';
		string cur;
		bool first=true;
		for(size_t k=0;k<=body.size();++k)
		{
			if(k==body.size()||body[k]==',')
			{
				auto pt=tok(cur);   //一个形参：pt[0] 是类型，后面是形参名
				if(!pt.empty())
				{
					if(!first)
						key+=',';
					key+=low(pt[0]);
					first=false;
				}
				cur.clear();
			}
			else
				cur+=body[k];
		}
		key+=')';
		S.insert(key);
	}
	cout<<S.size()<<'\n';
	return 0;
}
