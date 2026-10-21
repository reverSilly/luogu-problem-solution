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
	double x[25];
	i64 a[25];
	for(int i=0;i<n;++i)
		cin>>x[i]>>a[i];

	double hp=10;
	i64 exp=0;
	for(int i=0;i<n;++i)
	{
		hp-=x[i];
		if(hp<=0)
			break;
		hp=min(hp,10.0);
		exp+=a[i];
	}

	// 升到 L 级所需总经验是 2^L-1，L=floor(log2(exp+1))。与逐级扣减不同
	int lv=0;
	unsigned long long bound=1;
	unsigned long long ep1=(unsigned long long)exp+1ull;
	while((bound<<1)<=ep1)
	{
		bound<<=1;
		++lv;
	}
	i64 rem=exp-(((i64)bound)-1);
	cout<<lv<<' '<<rem<<'\n';
	return 0;
}
