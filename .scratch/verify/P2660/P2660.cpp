#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 gcd(i64 a,i64 b){return b?gcd(b,a%b):a;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 x,y;
	cin>>x>>y;
	// 最小边长和 = x+y-gcd(x,y)，体力 = 4 倍边长和
	cout<<4*(x+y-gcd(x,y))<<'\n';
	return 0;
}