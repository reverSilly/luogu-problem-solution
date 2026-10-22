// 题目：P2037 电话号码 —— 暴力：把号码存进数组后排序统计
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

char mp[26];
vector<string> v;

void init()
{
	const string g[8]={"ABC","DEF","GHI","JKL","MNO","PRS","TUV","WXY"};
	for(int i=0;i<8;++i)
		for(char c:g[i]) mp[c-'A']=char('2'+i);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	init();
	int N;
	cin>>N;
	for(int i=0;i<N;++i)
	{
		string raw,dig;
		cin>>raw;
		for(char c:raw)
		{
			if(c=='-') continue;
			if(c>='0'&&c<='9') dig+=c;
			else dig+=mp[c-'A'];
		}
		v.push_back(dig.substr(0,3)+"-"+dig.substr(3));
	}
	sort(v.begin(),v.end());
	bool any=false;
	for(int i=0;i<(int)v.size();)
	{
		int j=i;
		while(j<(int)v.size() && v[j]==v[i]) ++j;
		if(j-i>1)
		{
			cout<<v[i]<<' '<<j-i<<'\n';
			any=true;
		}
		i=j;
	}
	if(!any) cout<<"No duplicates.\n";
	return 0;
}
