#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n,m,k;
int a[mxn];

//统计一段 [l,r] 内不同宗教数
int types(int l,int r)
{
	bool seen[mxn+1]={};
	int typ=0;
	for(int i=l;i<=r;++i)
	{
		if(!seen[a[i]])
		{
			seen[a[i]]=true;
			++typ;
		}
	}
	return typ;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=1;i<=n;++i)
		cin>>a[i];

	//暴力：枚举所有连续划分（每种划分方案 = 在 n-1 个缝隙中选切点）
	int bestGrp=-1,bestRisk=-1;
	int cut[mxn];
	for(int mask=0;mask<(1<<(n-1));++mask)
	{
		//由 mask 还原切点（升序）
		int tot=0;
		for(int i=1;i<n;++i)
			if(mask>>(i-1)&1)
				cut[tot++]=i;
		cut[tot]=n;
		int grp=0,risk=0;
		bool ok=true;
		int prev=0;
		for(int i=0;i<tot;++i)
		{
			//这一组是 [prev+1, cut[i]]
			int typ=types(prev+1,cut[i]);
			if(typ>k)
			{
				ok=false;
				break;
			}
			risk+=typ;
			++grp;
			prev=cut[i];
		}
		if(!ok)
			continue;
		//最后一组 [prev+1, n]
		int typ=types(prev+1,n);
		if(typ>k)
			continue;
		risk+=typ;
		++grp;
		if(bestGrp==-1||grp<bestGrp)
			bestGrp=grp;
		if(bestRisk==-1||risk<bestRisk)
			bestRisk=risk;
	}
	cout<<bestGrp<<'\n';
	cout<<bestRisk<<'\n';
	return 0;
}
