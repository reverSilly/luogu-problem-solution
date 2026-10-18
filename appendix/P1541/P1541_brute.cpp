#include<bits/stdc++.h>
using namespace std;

int n,m;
int a[355],cnt[5];
map<array<int,4>,int> memo;

int dfs(int c1,int c2,int c3,int c4)
{
	int pos=c1+2*c2+3*c3+4*c4;
	if(c1==cnt[1]&&c2==cnt[2]&&c3==cnt[3]&&c4==cnt[4])
		return a[pos];
	array<int,4> st={c1,c2,c3,c4};
	auto it=memo.find(st);
	if(it!=memo.end())
		return it->second;
	int res=0;
	if(c1<cnt[1])
		res=max(res,dfs(c1+1,c2,c3,c4));
	if(c2<cnt[2])
		res=max(res,dfs(c1,c2+1,c3,c4));
	if(c3<cnt[3])
		res=max(res,dfs(c1,c2,c3+1,c4));
	if(c4<cnt[4])
		res=max(res,dfs(c1,c2,c3,c4+1));
	return memo[st]=res+a[pos];
}

int main()
{
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
	{
		int x;
		cin>>x;
		++cnt[x];
	}
	cout<<dfs(0,0,0,0)<<'\n';
	return 0;
}
