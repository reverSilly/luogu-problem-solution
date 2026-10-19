// P1515 暴力基准：枚举所有合法旅行方案(小 N)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{45};

int a[mxn],m,A,B;
i64 ans;

void dfs(int p)               // p=当前所在旅馆下标
{
	if(a[p]==7000)            // 到终点,记一种方案
	{
		++ans;
		return;
	}
	for(int j=p+1;j<m;++j)
		if(a[j]-a[p]>=A&&a[j]-a[p]<=B)
			dfs(j);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N;
	cin>>A>>B>>N;
	int base[14]={0,990,1010,1970,2030,2940,3060,3930,4060,4970,5030,5990,6010,7000};
	m=14;
	for(int i=0;i<14;++i)
		a[i]=base[i];
	for(int i=0,x;i<N;++i)
	{
		cin>>x;
		a[m++]=x;
	}
	sort(a,a+m);
	m=unique(a,a+m)-a;
	dfs(0);
	cout<<ans<<'\n';
	return 0;
}
