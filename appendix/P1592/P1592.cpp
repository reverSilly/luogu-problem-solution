//
// 题目：P1592 互质
// https://www.luogu.com.cn/problem/P1592
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,k;
int cop[1000005];               // cop[i]: 第 i 个与 n 互质的数(在 [1,n] 内, 0-based)
int cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;

	int phi=n;                  // 试除求欧拉函数
	int m=n;
	for(int p=2;(i64)p*p<=m;++p)
		if(m%p==0)
		{
			phi=phi/p*(p-1);
			while(m%p==0)
				m/=p;
		}
	if(m>1)
		phi=phi/m*(m-1);

	for(int i=1;i<=n;++i)
		if(gcd(i,n)==1)
			cop[cnt++]=i;

	int q=(k-1)/phi,r=(k-1)%phi;    // 周期性: 每 n 个整数恰有 phi 个互质
	i64 ans=(i64)q*n+cop[r];
	cout<<ans<<'\n';
	return 0;
}
