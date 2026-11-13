// 枚举所有 2^n 个子集, 统计最长的非降选取方案数
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int m;cin>>m;
	while(m--)
	{
		int n;cin>>n;
		vector<int>h(n);
		for(auto&x:h)cin>>x;
		int bt=0,bc=0;
		for(int S=0;S<(1<<n);++S)
		{
			vector<int>v;
			for(int i=0;i<n;++i)if(S>>i&1)v.push_back(h[i]);
			bool ok=1;
			for(int i=1;i<(int)v.size();++i)if(v[i]<v[i-1]){ok=0;break;}
			if(!ok)continue;
			int t=v.size();
			if(t>bt){bt=t;bc=1;}
			else if(t==bt)++bc;
		}
		printf("%d %d\n",bt,bc);
	}
	return 0;
}
