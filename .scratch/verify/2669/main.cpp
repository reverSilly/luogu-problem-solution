#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int k,ans=0;
	cin>>k;
	for(int n=1,day=0;day<k;++n)
	{
		int t=min(n,k-day);	// 第 n 组实际发放 t 天（最后一组可能被截断）
		ans+=n*t;
		day+=t;
	}
	cout<<ans<<'\n';
	return 0;
}