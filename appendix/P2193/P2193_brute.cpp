// P2193 暴力：DFS 枚举所有合法序列并计数（仅用于小规模对拍）
#include<bits/stdc++.h>
using namespace std;

int n,p;
long long ans=0;

void dfs(int len,int last)
{
	if(len==p)
	{
		++ans;
		return;
	}
	for(int i=1;i<=n;++i)
		if(i%last==0)dfs(len+1,i);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>p;
	for(int first=1;first<=n;++first)dfs(1,first);
	cout<<ans%1000000007<<'\n';
	return 0;
}
