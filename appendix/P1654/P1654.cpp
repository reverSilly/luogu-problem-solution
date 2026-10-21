//
// 题目：P1654 OSU!
// https://www.luogu.com.cn/problem/P1654
// AC
#include<bits/stdc++.h>
using namespace std;

int n;
double e1,e2,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		double p;
		cin>>p;
		ans+=p*(3*e2+3*e1+1);
		e2=p*(e2+2*e1+1);
		e1=p*(e1+1);
	}
	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}
