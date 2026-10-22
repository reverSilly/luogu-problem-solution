// 题目：P2108 学英语
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

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	getline(cin,line);

	int sgn=1;
	i64 cur=0,ans=0;
	string w;
	for(int i=0;i<(int)line.size();)
	{
		if(line[i]==' ')
		{
			++i;
			continue;
		}
		int j=i;
		while(j<(int)line.size() && line[j]!=' ') ++j;
		w=line.substr(i,j-i);
		i=j;
		if(w=="negative") sgn=-1;
		else if(w=="hundred") cur*=100;
		else if(w=="thousand")
		{
			ans+=cur*1000;
			cur=0;
		}
		else if(w=="million")
		{
			ans+=cur*1000000;
			cur=0;
		}
		else cur+=val(w);
	}
	cout<<sgn*(ans+cur)<<'\n';
	return 0;
}
