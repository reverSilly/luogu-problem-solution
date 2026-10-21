//
// 题目：P1835 素数密度
// https://www.luogu.com.cn/problem/P1835
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

bool isprime(i64 x)
{
	if(x<2)
		return false;
	for(i64 d=2;d*d<=x;++d)
		if(x%d==0)
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 L,R;
	cin>>L>>R;
	int ans=0;
	for(i64 x=L;x<=R;++x)
		if(isprime(x))
			++ans;
	cout<<ans<<'\n';
	return 0;
}
