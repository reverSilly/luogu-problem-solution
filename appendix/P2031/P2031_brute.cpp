// 题目：P2031 脑力达人之分割字串 —— 暴力：枚举 2^(n-1) 种切分方案
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305};

string s,wd[mxn];
int n,N,best=0;
vector<string> dic;

bool contain(const string &seg)
{
	for(auto &w:dic)
		if(seg.find(w)!=string::npos) return true;
	return false;
}

void dfs(int pos,int cnt)
{
	if(pos==n)
	{
		best=max(best,cnt);
		return;
	}
	for(int e=pos+1;e<=n;++e)
	{
		string seg=s.substr(pos,e-pos);
		if(contain(seg)) dfs(e,cnt+1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	cin>>N;
	n=s.size();
	for(int i=0;i<N;++i)
	{
		cin>>wd[i];
		dic.push_back(wd[i]);
	}
	dfs(0,0);
	cout<<best<<'\n';
	return 0;
}
