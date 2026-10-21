//
// 题目：P1828 [USACO3.2] 香甜的黄油 Sweet Butter
// https://www.luogu.com.cn/problem/P1828
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxp{45};
constexpr i64 inf{1145141919810LL};

int n,p,c;
int cnt[mxp];
i64 d[mxp][mxp];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>p>>c;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		--x;
		++cnt[x];
	}
	for(int i=0;i<p;++i)
		for(int j=0;j<p;++j)
			d[i][j]=(i==j?0:inf);
	for(int i=0,a,b,w;i<c;++i)
	{
		cin>>a>>b>>w;
		--a;--b;
		d[a][b]=min(d[a][b],(i64)w);
		d[b][a]=min(d[b][a],(i64)w);
	}
	for(int k=0;k<p;++k)
		for(int i=0;i<p;++i)
			if(d[i][k]<inf)
				for(int j=0;j<p;++j)
					if(d[k][j]<inf)
						d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
	i64 ans=inf;
	for(int s=0;s<p;++s)
	{
		i64 cur=0;
		bool ok=true;
		for(int i=0;i<p;++i)
			if(cnt[i])
			{
				if(d[i][s]>=inf)
				{
					ok=false;
					break;
				}
				cur+=cnt[i]*d[i][s];
			}
		if(ok)
			ans=min(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
