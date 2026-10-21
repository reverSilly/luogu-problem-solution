//
// 题目：P1651 塔
// 暴力基准：3^n DFS 枚举每块木块放入左塔、右塔或不放
#include<bits/stdc++.h>
using namespace std;

int n,h[55],suf[55],ans=-1;

void dfs(int i,int l,int r)
{
	if(l==r&&l>0)
		ans=max(ans,l);
	if(i==n)
		return;
	if(abs(l-r)>suf[i])
		return;
	if(max(l,r)+suf[i]/2<=ans)
		return;
	dfs(i+1,l+h[i],r);
	if(l!=r)
		dfs(i+1,l,r+h[i]);
	dfs(i+1,l,r);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>h[i];
	sort(h,h+n,greater<>());
	for(int i=n-1;i>=0;--i)
		suf[i]=suf[i+1]+h[i];
	dfs(0,0,0);
	cout<<ans<<'\n';
	return 0;
}
