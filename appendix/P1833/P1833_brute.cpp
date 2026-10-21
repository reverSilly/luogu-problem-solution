//
// 题目：P1833 樱花
// https://www.luogu.com.cn/problem/P1833
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n,T,ans;
int t[mxn],c[mxn],p[mxn];

void dfs(int i,int used,int val)
{
	if(i==n)
	{
		ans=max(ans,val);
		return;
	}
	int lim;
	if(t[i]<=0||t[i]>T-used)
		lim=0;
	else if(p[i]==0)
		lim=(T-used)/t[i];
	else
		lim=min(p[i],(T-used)/t[i]);
	for(int k=0;k<=lim;++k)
		dfs(i+1,used+k*t[i],val+k*c[i]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int h1,m1,h2,m2;
	char c1,c2;
	cin>>h1>>c1>>m1>>h2>>c2>>m2>>n;
	T=(h2*60+m2)-(h1*60+m1);
	if(T<0)
		T=0;
	for(int i=0;i<n;++i)
		cin>>t[i]>>c[i]>>p[i];
	dfs(0,0,0);
	cout<<ans<<'\n';
	return 0;
}
