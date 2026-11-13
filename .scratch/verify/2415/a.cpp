// 题目：P2415 集合求和
// 思路：每个元素恰出现在 2^(n-1) 个子集中（删/添该元素的双射），
//       所以答案 = 2^(n-1) * 所有元素之和。O(n)。
//       输入不给元素个数 ⇒ 读到 EOF；答案可达 1.6e13 ⇒ 全程 long long。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};

int n,a[mxn];
i64 sum,pw{1},ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>a[n])			// 元素个数不给出，读到 EOF 为止
	{
		sum+=a[n];
		++n;
	}
	for(int i=1;i<n;++i)		// pw=2^(n-1)；n==0 时 pw=1、sum=0，不会负数移位
		pw*=2;
	ans=sum*pw;
	cout<<ans<<'\n';
	return 0;
}
