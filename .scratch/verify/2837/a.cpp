// 题目：P2837 [USACO08FEB] Dining Cows B
// 合法终态 = 前 k 个 1 + 后 N-k 个 2；最少修改 = Hamming 距离。
// cost(k) = 前缀 2 数 + 后缀 1 数 = c1[N]+k-2*c1[k]，c1 为前缀 1 计数。
// 枚举 k=0..N 取 min，O(N)。k=0/N 覆盖"全 2 / 全 1"单组情形。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30005};

int n,d[mxn],c1[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>d[i];
	for(int i=0;i<n;++i)
		c1[i+1]=c1[i]+(d[i]==1);	// c1[k]=前 k 头里 1 的个数，c1[0]=0
	int ans=n;						// 上界：全改成 1 至多改 n 张
	for(int k=0;k<=n;++k)			// 枚举分界：前 k 头为 1，其余为 2
	{
		int cost=(k-c1[k])+(c1[n]-c1[k]);	// 前缀里的 2 + 后缀里的 1
		ans=min(ans,cost);
	}
	cout<<ans<<'\n';
	return 0;
}
