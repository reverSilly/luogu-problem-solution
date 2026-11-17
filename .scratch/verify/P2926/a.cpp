// 题目：P2926 [USACO08DEC] Patting Heads S
// 值域桶 cnt[v]；每个出现过的值 v 给它的全体倍数 m 贡献 cnt[v]（调和级数 O(V log V)）
// 奶牛 i 的答案 = sum[A_i]-1（A_i 整除自身，要扣掉自己这一拍）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005},mxv{1000005};

int n,a[mxn],cnt[mxv],sum[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		++cnt[a[i]];
	}
	for(int v=1;v<mxv;++v)
		if(cnt[v])
			for(int m=v;m<mxv;m+=v)
				sum[m]+=cnt[v];	// sum[m]=值整除 m 的奶牛总数
	for(int i=0;i<n;++i)
		cout<<sum[a[i]]-1<<'\n';
	return 0;
}