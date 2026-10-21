//
// 题目：P1591 阶乘数码
// https://www.luogu.com.cn/problem/P1591
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{3000};        // 1000! 只有 2568 位

int d[mxn],len;

void mul(int x)
{
	int carry=0;
	for(int i=0;i<len;++i)
	{
		int v=d[i]*x+carry;
		d[i]=v%10;
		carry=v/10;
	}
	while(carry)
	{
		d[len++]=carry%10;      // 进位可能不止一位
		carry/=10;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		int n,a;
		cin>>n>>a;
		d[0]=1;                 // 多测重新初始化
		len=1;
		for(int i=2;i<=n;++i)
			mul(i);
		int cnt=0;
		for(int i=0;i<len;++i)
			if(d[i]==a)
				++cnt;
		cout<<cnt<<'\n';
	}
	return 0;
}
