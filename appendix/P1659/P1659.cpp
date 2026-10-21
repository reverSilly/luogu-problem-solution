//
// 题目：P1659 [国家集训队] 拉拉队排练
// https://www.luogu.com.cn/problem/P1659
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr i64 p{19930726};

int n,d[mxn];
i64 k,cnt[mxn];
string s;

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	a%=p;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>s;

	for(int i=0,l=0,r=-1;i<n;++i)
	{
		int rad=(i>r)?1:min(d[l+r-i],r-i+1);
		while(i-rad>=0&&i+rad<n&&s[i-rad]==s[i+rad])
			++rad;
		d[i]=rad;
		++cnt[rad];
		if(i+rad-1>r)
		{
			l=i-rad+1;
			r=i+rad-1;
		}
	}

	int max_r=(n+1)/2;
	i64 ans=1;
	for(int r=max_r;r>=1&&k>0;--r)
	{
		cnt[r]+=cnt[r+1];
		i64 take=min(k,cnt[r]);
		ans=ans*qpow(2LL*r-1,take)%p;
		k-=take;
	}

	if(k>0)
		cout<<-1<<'\n';
	else
		cout<<ans<<'\n';
	return 0;
}
