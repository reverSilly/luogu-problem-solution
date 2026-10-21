//
// 题目：P1624 [ECNA 2005] 单词缩写 （暴力对拍用）
// 把短语中所有有效单词的字符按顺序摊平，DFS 决定每个字符是否被某个缩写字母使用
#include<bits/stdc++.h>
using namespace std;

int n;
set<string>bad;
string ab;
string ph;
struct node{char c;int w;};
vector<node>cs;
int wcnt;
long long ans;

void dfs(int p,int j,int mask)
{
	if(j==(int)ab.size())
	{
		if(mask==(1<<wcnt)-1)
			++ans;
		return;
	}
	if(p==(int)cs.size())
		return;
	dfs(p+1,j,mask);
	if(toupper(cs[p].c)==ab[j])
		dfs(p+1,j+1,mask|(1<<cs[p].w));
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
		getline(cin,line);
		while(line.empty())
			getline(cin,line);
		cs.clear();
		wcnt=0;
		istringstream is(line);
		string w;
		while(is>>w)
		{
			if(bad.count(w))
				continue;
			for(char c:w)
				cs.push_back({c,wcnt});
			++wcnt;
		}
		ans=0;
		dfs(0,0,0);
		if(ans>0)
			cout<<ab<<" can be formed in "<<ans<<" ways\n";
		else
			cout<<ab<<" is not a valid abbreviation\n";
	}
	return 0;
}
