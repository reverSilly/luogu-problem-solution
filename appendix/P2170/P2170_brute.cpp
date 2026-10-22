// P2170 暴力：枚举所有 2^n 选人方案，判断是否合法，取距 m 最近的人数
#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n,m,k;
	scanf("%d%d%d",&n,&m,&k);
	vector<pair<int,int>> e(k);
	for(int i=0;i<k;++i)
		scanf("%d%d",&e[i].first,&e[i].second);
	int best=0,bd=1000000000;
	for(int s=0;s<(1<<n);++s)
	{
		bool ok=true;
		for(auto &pr:e)
			if(((s>>(pr.first-1))&1)!=((s>>(pr.second-1))&1))
			{
				ok=false;
				break;
			}
		if(!ok)
			continue;
		int c=__builtin_popcount((unsigned)s);
		// 差相同取较小者：必须显式比较 c
		if(abs(c-m)<bd||(abs(c-m)==bd&&c<best))
			bd=abs(c-m),best=c;
	}
	printf("%d\n",best);
	return 0;
}
