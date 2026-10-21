//
// 题目：P1881 [USACO12FEB] 绳子对折 B
// https://www.luogu.com.cn/problem/P1881
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int d[mxn];

bool pal(int l,int r)
{
	while(l<r)
	{
		if(d[l]!=d[r])
			return false;
		++l;
		--r;
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,L;
	cin>>n>>L;
	vector<int> x(n);
	for(int i=0;i<n;++i)
		cin>>x[i];
	sort(x.begin(),x.end());
	for(int i=0;i+1<n;++i)
		d[i]=x[i+1]-x[i];
	int m=n-1;
	int ans=0;
	for(int i=0;i<m;++i)
		if(pal(0,i))
			++ans;
	for(int i=1;i<m;++i)
		if(pal(i,m-1))
			++ans;
	cout<<ans<<'\n';
	return 0;
}
