//
// 题目：P1531 I Hate It
// 暴力基准: 直接维护原数组, 每次询问线性扫描 [l, r] 取最大值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int n,m,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
	{
		char op;
		int x,y;
		cin>>op>>x>>y;
		if(op=='Q')
		{
			int mx=a[x-1];
			for(int j=x-1;j<y;++j)
				mx=max(mx,a[j]);
			cout<<mx<<'\n';
		}
		else
			a[x-1]=max(a[x-1],y);
	}
	return 0;
}
