//
// 题目：P1869 愚蠢的组合数
// https://www.luogu.com.cn/problem/P1869
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		i64 n,k;
		cin>>n>>k;
		cout<<(((n&k)==k)?1:0)<<'\n';
	}
	return 0;
}
