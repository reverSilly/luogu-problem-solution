// 题目：P2663 越越的组队
// 思路：恰好选 n/2 人、总和 ≤ ⌊sum/2⌋、最大化总和 ⇒ 带个数限制的 0/1 背包。
//       f[j][s]：前缀里恰好选 j 人、和恰为 s 是否可行。
//       逐个物品滚动，j 倒序保证读到的 f[j-1] 还是旧值（每人最多选一次）。
//       答案 = 最大的 s ≤ sum/2 使 f[n/2][s] 为真（取最小的一半人保证可行）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxs{5005};

int n,a[mxn],sum,half,ans;
bool f[mxn][mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		sum+=a[i];
	}
	half=sum/2;
	f[0][0]=true;
	for(int i=0;i<n;++i)
		for(int j=min(i+1,n/2);j>=1;--j)
			for(int s=half;s>=a[i];--s)
				f[j][s]=f[j][s]||f[j-1][s-a[i]];
	for(int s=half;s>=0;--s)
		if(f[n/2][s])
		{
			ans=s;
			break;
		}
	cout<<ans<<'\n';
	return 0;
}