#include<bits/stdc++.h>
using namespace std;

int n;
long long m;
vector<int> adj[100005];
long long c[100005],l_val[100005];

void collect(int u,vector<long long>& vec)
{
	vec.push_back(c[u]);
	for(int v:adj[u])
		collect(v,vec);
}

int main()
{
	cin>>n>>m;
	for(int i=1;i<=n;++i)
	{
		int b;
		cin>>b>>c[i]>>l_val[i];
		if(b>0)
			adj[b].push_back(i);
	}
	long long ans=0;
	for(int i=1;i<=n;++i)
	{
		vector<long long> vec;
		collect(i,vec);
		sort(vec.begin(),vec.end());
		long long s=0,cnt=0;
		for(long long x:vec)
			if(s+x<=m)
			{
				s+=x;
				++cnt;
			}
		ans=max(ans,cnt*l_val[i]);
	}
	cout<<ans<<'\n';
	return 0;
}
