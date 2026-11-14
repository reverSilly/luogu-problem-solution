#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int m,n,a[mxn],b[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		vis[a[i]]=true;
	}
	int cnt=0;
	for(int i=1;i<=n*m;++i)		// 补集：不在 John 手里的牌
		if(!vis[i])
			b[cnt++]=i;
	sort(a,a+n);
	sort(b,b+cnt);
	int j=0,beat=0;
	for(int i=0;i<n;++i)		// 贪心：每张 John 的牌配一张最小的能压过它的牌
	{
		while(j<cnt&&b[j]<=a[i])
			++j;
		if(j<cnt)
		{
			++beat;
			++j;
		}
	}
	cout<<n-beat<<'\n';			// 压不掉的牌数 = 最少赢的轮数
	return 0;
}