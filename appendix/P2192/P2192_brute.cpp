// P2192 暴力：枚举所有卡片子集及其所有排列，取能被 90 整除的最大值
#include<bits/stdc++.h>
using namespace std;

int n;
vector<int> a;

long long best=-1;

void dfs(int mask,vector<int>&cur)
{
	if(cur.size())
	{
		// 枚举 cur 的所有不同排列
		vector<int> t=cur;
		sort(t.begin(),t.end());
		do
		{
			long long v=0;
			for(int x:t)v=v*10+x;
			if(v%90==0&&v>best)best=v;
		}while(next_permutation(t.begin(),t.end()));
	}
	for(int i=0;i<(int)a.size();++i)
		if(!(mask>>i&1))
		{
			cur.push_back(a[i]);
			dfs(mask|(1<<i),cur);
			cur.pop_back();
		}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	a.resize(n);
	for(int&x:a)cin>>x;
	vector<int> cur;
	dfs(0,cur);
	if(best<0)cout<<-1<<'\n';
	else cout<<best<<'\n';
	return 0;
}
