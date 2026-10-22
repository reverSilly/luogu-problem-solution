// 题目：P2108 学英语 —— 暴力：按 million/thousand 分段, 每段单独求值
#include<bits/stdc++.h>
using namespace std;

using i64=long long;

const char* word[28]={"zero","one","two","three","four","five","six","seven","eight","nine",
	"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen",
	"twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

int val(const string &w)
{
	for(int i=0;i<28;++i)
		if(w==word[i])
			return i<20?i:(i-18)*10;
	return -1;
}

// 求一段(不含 million/thousand)的值: 形如 "X hundred Y" 或 "X" 或 "Y"
i64 seg_value(const vector<string> &w,int l,int r)
{
	i64 hundredPart=0,rest=0;
	int pos=-1;
	for(int i=l;i<r;++i)
		if(w[i]=="hundred") pos=i;
	if(pos==-1)
	{
		for(int i=l;i<r;++i) rest+=val(w[i]);
		return rest;
	}
	for(int i=l;i<pos;++i) hundredPart+=val(w[i]);
	for(int i=pos+1;i<r;++i) rest+=val(w[i]);
	return hundredPart*100+rest;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	getline(cin,line);
	vector<string> w;
	string t;
	for(char c:line)
	{
		if(c==' ') { if(!t.empty()) { w.push_back(t); t.clear(); } }
		else t+=c;
	}
	if(!t.empty()) w.push_back(t);

	int sgn=1;
	if(!w.empty() && w[0]=="negative")
	{
		sgn=-1;
		w.erase(w.begin());
	}

	// 找出 million / thousand 的位置, 分成三段
	int im=-1,it=-1;
	for(int i=0;i<(int)w.size();++i)
	{
		if(w[i]=="million" && im==-1) im=i;
		if(w[i]=="thousand" && it==-1) it=i;
	}

	i64 ans=0;
	if(im!=-1)
	{
		ans+=seg_value(w,0,im)*1000000;
		if(it!=-1) { ans+=seg_value(w,im+1,it)*1000; ans+=seg_value(w,it+1,(int)w.size()); }
		else ans+=seg_value(w,im+1,(int)w.size());
	}
	else if(it!=-1)
	{
		ans+=seg_value(w,0,it)*1000;
		ans+=seg_value(w,it+1,(int)w.size());
	}
	else ans+=seg_value(w,0,(int)w.size());

	cout<<sgn*ans<<'\n';
	return 0;
}
