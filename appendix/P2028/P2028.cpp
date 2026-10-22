// 题目：P2028 龙兄摘苹果
// 第二类斯特林数 S(n,k)：S(i,j)=S(i-1,j-1)+j*S(i-1,j)，模 p 递推
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using u64=unsigned long long;
using u128=__uint128_t;

constexpr int mxn{10005};

int n,k;
u64 p;
u64 f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>p;
	f[0]=1%p;
	for(int i=1;i<=n;++i)
	{
		int up=min(i,k);
		for(int j=up;j>=1;--j)
		{
			//f[j]=f[j-1]+j*f[j]，用 128 位防溢出
			u128 val=(u128)j*f[j]+f[j-1];
			f[j]=(u64)(val%p);
		}
		f[0]=0;
	}
	if(k>n)
		cout<<0<<'\n';
	else
		cout<<f[k]<<'\n';
	return 0;
}
