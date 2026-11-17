// 题目：P3048 [USACO12FEB] Cow IDs S
// 第N个popcount=K的数：先按位数定位（长度L共C(L-1,K-1)个），再逐位组合数反排名
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxp{5005},mxk{15};
constexpr i64 cap{2000000000};	// 截断上界>N即可：不改变任何比较/减法结果

int n,k;
i64 C[mxp][mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	if(k==1)	// 编号即2^(n-1)：1后跟n-1个0，位数可达1e7，特判
	{
		cout<<'1'<<string(n-1,'0')<<'\n';
		return 0;
	}
	for(int p=0;p<mxp;++p)
	{
		C[p][0]=1;
		for(int j=1;j<=min(p,k);++j)
			C[p][j]=min(cap,C[p-1][j-1]+C[p-1][j]);
	}
	int L=k,r=n;
	for(i64 cnt=C[L-1][k-1];cnt<r;cnt=C[L-1][k-1])
	{
		r-=cnt;
		++L;
	}
	string ans(L,'0');
	ans[0]='1';	// 最高位必为1
	int rem=k-1;
	for(int p=L-2;p>=0;--p)
	{
		if(rem==0)
			break;	// 剩余位全0
		if(rem==p+1)	// 剩余位全1
		{
			for(int q=0;q<=p;++q)
				ans[L-1-q]='1';
			break;
		}
		if(r<=C[p][rem])
			continue;	// 该位填0时方案数已够
		r-=C[p][rem];
		--rem;
		ans[L-1-p]='1';
	}
	cout<<ans<<'\n';
	return 0;
}
