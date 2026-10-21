//
// 题目：P1621 集合
// https://www.luogu.com.cn/problem/P1621
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int a,b,p,ans;
int f[mxn];
bool comp[mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b>>p;
	for(int i=a;i<=b;++i)
		f[i]=i;
	for(int i=2;i<=b;++i)
	{
		if(comp[i])
			continue;
		for(i64 j=(i64)i*i;j<=b;j+=i)   // i 是质数
			comp[j]=true;
		if(i<p)
			continue;
		int st=((a+i-1)/i)*i;           // [a,b] 中最小的 i 的倍数
		if(st>b)
			continue;
		for(int m=st+i;m<=b;m+=i)
		{
			int u=getrt(st),v=getrt(m);
			if(u!=v)
				f[u]=v;
		}
	}
	for(int i=a;i<=b;++i)
		if(getrt(i)==i)
			++ans;
	cout<<ans<<'\n';
	return 0;
}
