// 题目：P2035 [USACO08JAN] iCow B
// 逐轮模拟：选最大权值（并列取最小编号），再把它的权值平均分给其他曲子
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,T;
int r[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>T;
	for(int i=1;i<=n;++i) cin>>r[i];

	for(int step=0;step<T;++step)
	{
		// 选权值最大的曲子，并列时取编号最小的
		int best=1;
		for(int i=2;i<=n;++i)
			if(r[i]>r[best]) best=i;
		cout<<best<<'\n';
		// 把 best 的权值分给其余 N-1 首曲子
		int w=r[best];
		r[best]=0;
		if(n>1)	// N=1 时没有别的曲子可分，避免除以 0
		{
			int q=w/(n-1),rem=w%(n-1);
			for(int i=1;i<=n;++i)
			{
				if(i==best) continue;
				r[i]+=q;
				if(rem>0) { r[i]++; rem--; }	// 余数按编号从小到大依次多给 1
			}
		}
	}
	return 0;
}
