// 题目：P2907 [USACO08OPEN] Roads Around The Farm S
// 能分裂 ⟺ x≥K+2 且 x≡K (mod 2)；分裂方式唯一：(x+K)/2 与 (x-K)/2
// 答案=分裂树的叶子数，直接递归；m=(x-K)/2 坐标下两孩子都减半 ⇒ 深度≤30
#include<bits/stdc++.h>
using namespace std;

int n,k;

int dfs(int x)
{
	if(x<k+2||(x-k)&1)	// 不能分裂：安静吃草，贡献 1 个群体
		return 1;
	return dfs((x+k)/2)+dfs((x-k)/2);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	cout<<dfs(n)<<'\n';
	return 0;
}
