// 题目：P2902 Pearl Pairing G
// 按颜色展开成有序序列（计数排序，O(N)），第 i 颗与第 i+N/2 颗配对
// 可行性保证 max C_i<=N/2 ⇒ 同色块长<=N/2 ⇒ 错位半周期必异色
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,c,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>c;
	for(int i=0,k,cnt=0;i<c;++i)
	{
		cin>>k;
		while(k--)
			a[cnt++]=i;	// 按颜色顺序展开，天然有序
	}
	for(int i=0;i<n/2;++i)
		cout<<a[i]+1<<' '<<a[i+n/2]+1<<'\n';
	return 0;
}