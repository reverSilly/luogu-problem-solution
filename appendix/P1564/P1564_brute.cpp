#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30};

int n,m;
int a[mxn];
int ans;

void dfs(int pos,int rooms)
{
	if(rooms>=ans)                // 剪枝
		return;
	if(pos==n)
	{
		ans=min(ans,rooms);
		return;
	}
	int c1=0,c2=0;
	for(int j=pos;j<n;++j)
	{
		if(a[j]==1)
			++c1;
		else
			++c2;
		if(c1==0||c2==0||abs(c1-c2)<=m)
			dfs(j+1,rooms+1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	ans=n;                        // 每个单独成房一定可行
	dfs(0,0);
	cout<<ans<<'\n';
	return 0;
}
