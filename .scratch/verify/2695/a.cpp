// 题目：P2695 骑士的工作
// 思路：头与骑士都升序。先处理最小头，z<a_i 的骑士对所有更大头都太弱⇒永久丢弃。
//       双指针：每个头配最便宜可用骑士（交换论证证最优），j==m 即无解。
//       花费上界 n*maxz=2e9 逼近 int 上限，用 i64。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005};

int n,m,a[mxn],z[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
		cin>>z[i];
	sort(a,a+n);
	sort(z,z+m);
	i64 ans=0;
	int j=0;
	for(int i=0;i<n;++i)
	{
		while(j<m&&z[j]<a[i])	// 弱骑士对当前及以后所有头都无用，丢弃
			++j;
		if(j==m)
		{
			cout<<"you died!\n";
			return 0;
		}
		ans+=z[j];	// 最便宜可用骑士配当前最小头
		++j;
	}
	cout<<ans<<'\n';
	return 0;
}
