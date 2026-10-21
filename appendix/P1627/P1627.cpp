//
// 题目：P1627 [CQOI2009] 中位数
// https://www.luogu.com.cn/problem/P1627
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,b,a[mxn],v[mxn],pre[mxn];
int cl[mxn*2],cr[mxn*2];                // 前缀和计数，下标平移 n

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>b;
	int pos=-1;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		if(a[i]==b)
			pos=i;
	}
	for(int i=0;i<n;++i)
		v[i]=(a[i]>b)-(a[i]<b);         // 大于 b 记 +1，小于记 -1，b 记 0
	for(int i=0;i<n;++i)
		pre[i+1]=pre[i]+v[i];
	for(int l=0;l<=pos;++l)
		++cl[pre[l]+n];
	for(int q=pos+1;q<=n;++q)
		++cr[pre[q]+n];
	i64 ans=0;
	for(int x=0;x<=2*n;++x)
		ans+=(i64)cl[x]*cr[x];
	cout<<ans<<'\n';
	return 0;
}
