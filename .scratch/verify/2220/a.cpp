// 题目：P2220 [HAOI2012] 容易题
// 答案 = 每个位置「允许取值之和」的乘积；未受限位置因子为 n(n+1)/2
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxk{100005};
constexpr i64 p{1'000'000'007};

i64 n,m;
int k;
pair<i64,i64>c[mxk];

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
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

	cin>>n>>m>>k;
	for(int i=0;i<k;++i)
		cin>>c[i].first>>c[i].second;
	sort(c,c+k);
	i64 S=(n%p)*((n+1)%p)%p*500000004%p;   // n(n+1)/2 mod p，2 的逆元
	i64 ans=1;
	int d=0;
	for(int i=0;i<k;)
	{
		int j=i;
		i64 sum=0;
		while(j<k&&c[j].first==c[i].first)
		{
			// 同组内相同的 y 只扣一次
			if(j==i||c[j].second!=c[j-1].second)
				sum=(sum+c[j].second)%p;
			++j;
		}
		ans=ans*((S-sum+p)%p)%p;
		++d;
		i=j;
	}
	ans=ans*qpow(S,m-d)%p;
	cout<<ans<<'\n';
	return 0;
}
