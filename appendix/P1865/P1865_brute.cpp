//
// 题目：P1865 A % B Problem
// https://www.luogu.com.cn/problem/P1865
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

bool prime(int x)
{
	if(x<2)
		return false;
	for(int d=2;1ll*d*d<=x;++d)
		if(x%d==0)
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		i64 l,r;
		cin>>l>>r;
		if(l<1||r>m)
		{
			cout<<"Crossing the line\n";
			continue;
		}
		int cnt=0;
		for(int x=(int)l;x<=(int)r;++x)
			if(prime(x))
				++cnt;
		cout<<cnt<<'\n';
	}
	return 0;
}
