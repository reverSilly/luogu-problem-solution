// 题目：P2476 [SCOI2008] 着色方案
// 状态 (a1..a5,last)：aj=剩余次数恰为 j 的颜色种数，last=上一个用的颜色还剩几次
// 同剩余次数的颜色完全对称，方案数只依赖该状态；记忆化搜索
// 转移：选剩余 j 次的颜色，共 a[j]-[j==last] 种，用掉一次后落入 j-1 桶，新 last=j-1
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxc{16},mxl{6};
constexpr int p{1'000'000'007};
constexpr int inf{1145141919};

int k,c[20],b[mxl];
int dp[mxc][mxc][mxc][mxc][mxc][mxl];

int dfs(int a1,int a2,int a3,int a4,int a5,int last)
{
	if(a1+a2+a3+a4+a5==0)
		return 1;   // 油漆用完，空方案恰一种
	int&res=dp[a1][a2][a3][a4][a5][last];
	if(res!=inf)
		return res;
	res=0;
	int a[mxl]={0,a1,a2,a3,a4,a5};
	for(int j=1;j<=5;++j)
	{
		int cnt=a[j]-(j==last);   // 上一个颜色若还剩 j 次，它在 j 桶里但不能选
		if(cnt<=0)
			continue;
		--a[j];
		if(j>1)
			++a[j-1];
		res=(res+(i64)cnt*dfs(a[1],a[2],a[3],a[4],a[5],j-1))%p;
		if(j>1)
			--a[j-1];
		++a[j];
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	fill_n(&dp[0][0][0][0][0][0],mxc*mxc*mxc*mxc*mxc*mxl,inf);
	cin>>k;
	for(int i=0;i<k;++i)
	{
		cin>>c[i];
		++b[c[i]];
	}
	cout<<dfs(b[1],b[2],b[3],b[4],b[5],0)<<'\n';
	return 0;
}
