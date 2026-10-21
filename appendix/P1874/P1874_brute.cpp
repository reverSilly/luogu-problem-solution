//
// 题目：P1874 快速求和
// https://www.luogu.com.cn/problem/P1874
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int inf{1145141919};

string s;
int tar,len,ans;
int suf[45];

void dfs(int pos,i64 sum,int parts)
{
	if(sum>tar)
		return;
	if(pos==len)
	{
		if(sum==tar)
			ans=min(ans,parts-1);
		return;
	}
	if(parts>=ans)
		return;
	if(sum+suf[pos]>tar)
		return;
	i64 val=0;
	for(int j=pos;j<len;++j)
	{
		val=val*10+(s[j]-'0');
		if(val>tar)
			break;
		dfs(j+1,sum+(int)val,parts+1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>s>>tar;
	len=(int)s.size();
	suf[len]=0;
	for(int i=len-1;i>=0;--i)
		suf[i]=suf[i+1]+(s[i]-'0');
	ans=inf;
	dfs(0,0,0);
	if(ans>=inf)
		cout<<-1<<'\n';
	else
		cout<<ans<<'\n';
	return 0;
}
