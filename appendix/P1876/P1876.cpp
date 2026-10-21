//
// 题目：P1876 开灯
// https://www.luogu.com.cn/problem/P1876
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	cin>>n;
	bool fst=true;
	for(i64 i=1;i<=n/i;++i)
	{
		if(!fst)
			cout<<' ';
		fst=false;
		cout<<i*i;
	}
	cout<<'\n';
	return 0;
}
