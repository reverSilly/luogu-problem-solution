// 题目：P2089 烤鸡
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{59055};
constexpr int mxs{11};

int n;
int cur[mxs];
vector<array<int,10>> sol;

void dfs(int dep,int sum)
{
	if(dep>10)
	{
		if(sum==n)
			sol.push_back({cur[1],cur[2],cur[3],cur[4],cur[5],cur[6],cur[7],cur[8],cur[9],cur[10]});
		return;
	}
	// 每种配料 1~3 克，按字典序从小到大枚举
	for(int v=1;v<=3;++v)
	{
		cur[dep]=v;
		dfs(dep+1,sum+v);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	dfs(1,0);
	cout<<sol.size()<<'\n';
	for(auto&s:sol)
	{
		for(int i=0;i<10;++i)
			cout<<s[i]<<(i==9?'\n':' ');
	}
	return 0;
}
