// 题目：P2525 Uim的情人节礼物·其之壱
// 思路：prev_permutation 四步构造字典序前驱：
//       1) 从右找第一个下降位 i（a[i]>a[i+1]）；没有 ⇒ 已是升序（第一个）⇒ ERROR
//       2) 后缀升序 ⇒ 从右找第一个 a[j]<a[i]，即"小于 a[i] 的最大值"
//       3) 交换 a[i],a[j]（后缀仍升序）
//       4) 反转后缀 ⇒ 降序 = 剩余元素的最大排列
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int i=n-2;
	while(i>=0&&a[i]<=a[i+1])
		--i;
	if(i<0)
	{
		cout<<"ERROR\n";
		return 0;
	}
	int j=n-1;
	while(a[j]>=a[i])
		--j;
	swap(a[i],a[j]);
	reverse(a+i+1,a+n);
	for(int k=0;k<n;++k)
		cout<<a[k]<<(k+1==n?'\n':' ');
	return 0;
}
