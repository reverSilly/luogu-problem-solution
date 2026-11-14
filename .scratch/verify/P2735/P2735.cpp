#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,m,p;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m>>p;
	i64 area2=p*m;					// 2A：底 p 高 m
	i64 b=gcd(n,m)+gcd(abs(n-p),m)+p;	// 边界格点数 B（三边 gcd 之和）
	cout<<(area2-b)/2+1<<'\n';		// Pick：I=A-B/2+1
	return 0;
}