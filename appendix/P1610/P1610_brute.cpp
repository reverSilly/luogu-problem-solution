//
// P1610 暴力（对拍用）：n<=11 时枚举哪些灯保留, 逐块检查合法性, 取最大删除数
// 合法判据：若两个"相邻保留灯" a<b 之间有关掉的灯, 则必须 p[b]-p[a] <= dist
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,ans,mxdel;
i64 d,p[25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>d;
	for(int i=0;i<n;++i)
		cin>>p[i];
	sort(p,p+n);
	if(n<=2)
	{
		cout<<0<<'\n';
		return 0;
	}
	int inter=n-2;
	for(int mask=0;mask<(1<<inter);++mask)
	{
		vector<int>keep;
		keep.push_back(0);
		for(int i=0;i<inter;++i)
			if(mask>>i&1)
				keep.push_back(i+1);
		keep.push_back(n-1);
		bool ok=true;
		for(int j=0;j+1<(int)keep.size();++j)
			if(keep[j+1]>keep[j]+1&&p[keep[j+1]]-p[keep[j]]>d)
			{
				ok=false;
				break;
			}
		if(ok)
			mxdel=max(mxdel,n-(int)keep.size());
	}
	cout<<mxdel<<'\n';
	return 0;
}
