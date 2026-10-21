//
// 题目：P1867 【Mc生存】经验值
// https://www.luogu.com.cn/problem/P1867
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	double hp=10;
	i64 exp=0;
	for(int i=0;i<n;++i)
	{
		double x;
		i64 a;
		cin>>x>>a;
		hp-=x;
		if(hp<=0)
			break;
		if(hp>10)
			hp=10;
		exp+=a;
	}
	int lv=0;
	i64 need=1;
	while(exp>=need)
	{
		exp-=need;
		++lv;
		need<<=1;
	}
	cout<<lv<<' '<<exp<<'\n';
	return 0;
}
