// P2190 暴力：逐条订票把人数直接加到经过的每一段上，最后取最大段载客量
#include<bits/stdc++.h>
using namespace std;

int a[1000005];

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	for(int i=0;i<m;++i)
	{
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		// 从 x 顺时针到 y，经过第 x,x+1,...,y-1 段（环形）
		int s=x;
		while(s!=y)
		{
			a[s]+=z;
			s=(s==n?1:s+1);
		}
	}
	int mx=0;
	for(int i=1;i<=n;++i)
		mx=max(mx,a[i]);
	printf("%d\n",(mx+35)/36);
	return 0;
}
