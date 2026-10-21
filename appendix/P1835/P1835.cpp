//
// 题目：P1835 素数密度
// https://www.luogu.com.cn/problem/P1835
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxp{50005};
constexpr int mxlen{1000005};

bool comp[mxp];
int prime[mxp],pc;
bool bad[mxlen];

void sieve()
{
	for(int i=2;i<mxp;++i)
	{
		if(!comp[i])
			prime[pc++]=i;
		for(int j=0;j<pc&&i*1LL*prime[j]<mxp;++j)
		{
			comp[i*prime[j]]=true;
			if(i%prime[j]==0)
				break;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	sieve();
	i64 L,R;
	cin>>L>>R;
	int len=(int)(R-L+1);
	for(int i=0;i<pc;++i)
	{
		i64 p=prime[i];
		if(p*p>R)
			break;
		i64 st=(L+p-1)/p*p;
		if(st<p*p)
			st=p*p;
		for(i64 x=st;x<=R;x+=p)
			bad[(int)(x-L)]=true;
	}
	int ans=0;
	for(int i=0;i<len;++i)
		if(L+(i64)i>=2&&!bad[i])
			++ans;
	cout<<ans<<'\n';
	return 0;
}
