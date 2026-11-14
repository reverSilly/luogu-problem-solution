#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N;
	cin>>N;			// N = 2n
	int n=N/2,m=n-1;
	long double p=1;		// p = C(2m,m)/4^m，即“队尾两人不同”的概率
	for(int i=1;i<=m;++i)
		p=p*(m+i)/i/4;		// 累乘中心二项式系数，每步除以 4 防溢出
	cout<<fixed<<setprecision(4)<<1-p<<'\n';
	return 0;
}