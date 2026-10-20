#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};

int n;
int U[mxn],D[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>U[i]>>D[i];
	vector<int> up(n),dn(n);
	iota(up.begin(),up.end(),0);
	i64 ans{-1};
	do
	{
		i64 top[mxn],t{0};
		for(int k=0;k<n;++k)
		{
			t+=U[up[k]];
			top[up[k]]=t;         // 该奶牛到达山顶的时刻
		}
		iota(dn.begin(),dn.end(),0);
		do
		{
			i64 cur{0};
			for(int k=0;k<n;++k)
			{
				int c=dn[k];
				cur=max(cur,top[c]);   // 唐农夫空闲时刻 与 到顶时刻 取较晚
				cur+=D[c];
			}
			if(ans<0||cur<ans)
				ans=cur;
		}while(next_permutation(dn.begin(),dn.end()));
	}while(next_permutation(up.begin(),up.end()));
	cout<<ans<<'\n';
	return 0;
}
