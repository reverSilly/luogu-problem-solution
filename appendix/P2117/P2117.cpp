// 题目：P2117 小Z的矩阵
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,q;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	// G(A) mod 2 等于对角线元素之和 mod 2, 只需留对角线
	int g=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			int v;
			cin>>v;
			if(i==j) g^=v;
		}

	// 每次翻转行/列都恰好翻转一个对角元素, 即奇偶性取反
	string out;
	for(int k=0;k<q;++k)
	{
		int op;
		cin>>op;
		if(op==3) out+=char('0'+g);
		else
		{
			int x;
			cin>>x;	// 翻哪一行/列不影响结论, 但必须读掉
			g^=1;
		}
	}
	cout<<out<<'\n';
	return 0;
}
