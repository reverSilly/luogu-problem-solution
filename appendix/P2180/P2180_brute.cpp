// P2180 暴力：枚举网格点集的全部 2^(N*M) 个子集，取恰好 K 枚石子的那些，
//       逐个数出四角都有石子的轴平行矩形（任意两行两列），取最大值
#include<bits/stdc++.h>
using namespace std;

int n,m,k;

int main()
{
	scanf("%d%d%d",&n,&m,&k);
	int tot=n*m;
	unsigned full=(1u<<tot)-1;
	int best=0;
	for(unsigned s=full;s;s=(s-1)&full)
	{
		if(__builtin_popcount(s)!=k)
			continue;
		int cnt=0;
		for(int i1=0;i1<n;++i1)
			for(int i2=i1+1;i2<n;++i2)
				for(int j1=0;j1<m;++j1)
					for(int j2=j1+1;j2<m;++j2)
					{
						int p=i1*m+j1,q=i1*m+j2,r=i2*m+j1,s2=i2*m+j2;
						if((s>>p&1)&&(s>>q&1)&&(s>>r&1)&&(s>>s2&1))
							++cnt;
					}
		if(cnt>best)
			best=cnt;
	}
	printf("%d\n",best);
	return 0;
}
