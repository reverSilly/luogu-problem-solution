// 题目：P2623 物品选取
// 混合背包：甲=泛化物品(分组背包,容量倒序) / 乙=多重(二进制拆分) / 丙=完全(正序)
// f[j]=体积不超过 j 的最大价值，全 0 初始化，答案 f[m]
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{2005};

int n,m;
i64 f[mxm],v[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		int x;cin>>x;
		if(x==1)	// 甲：泛化物品，体积 t 任选一个，价值 A*t^2-B*t
		{
			i64 A,B;cin>>A>>B;
			for(int t=0;t<=m;++t)
				v[t]=A*t*t-B*t;
			for(int j=m;j>=0;--j)	// 倒序枚举容量：本物品只被选一次
				for(int t=1;t<=j;++t)
					f[j]=max(f[j],f[j-t]+v[t]);
		}
		else if(x==2)	// 乙：多重背包，二进制拆分成 01 物品
		{
			i64 A,B,C;cin>>A>>B>>C;
			for(i64 k=1;C>0;k<<=1)
			{
				i64 t=min(k,C);C-=t;
				i64 vol=B*t,val=A*t;
				for(int j=m;j>=vol;--j)
					f[j]=max(f[j],f[j-vol]+val);
			}
		}
		else	// 丙：完全背包，题目保证 B>0
		{
			i64 A,B;cin>>A>>B;
			for(int j=B;j<=m;++j)
				f[j]=max(f[j],f[j-B]+A);
		}
	}
	cout<<f[m]<<'\n';
	return 0;
}
