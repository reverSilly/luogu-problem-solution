// 题目：P2249 【深基13.例1】查找
// 思路：单调不减 ⇒ 谓词 a[i]>=q 前假后真
//       二分第一个 >=q 的位置（相等也不停），再检查该位置是否真的等于 q
//       n<=1e6，必须关闭流同步
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int n,m,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0,q;i<m;++i)
	{
		cin>>q;
		int l=0,r=n;
		while(l<r)
		{
			int mid=(l+r)>>1;
			if(a[mid]>=q)
				r=mid;
			else
				l=mid+1;
		}
		// l 是第一个 >=q 的位置（可能是 n）
		if(l<n&&a[l]==q)
			cout<<l+1<<' ';
		else
			cout<<-1<<' ';
	}
	cout<<'\n';
	return 0;
}
