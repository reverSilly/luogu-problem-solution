//
// 题目：P1847 轰炸II 暴力（按轰炸轮次累加每个点）
// https://www.luogu.com.cn/problem/P1847
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

i64 x1s[mxn],y1s[mxn],x2s[mxn],y2s[mxn];
i64 px[mxn],py[mxn];
int cnt[mxn],last[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int m,n;
	cin>>m>>n;
	for(int i=0;i<m;++i)
	{
		cin>>x1s[i]>>y1s[i]>>x2s[i]>>y2s[i];
		if(x1s[i]>x2s[i])
			swap(x1s[i],x2s[i]);
		if(y1s[i]>y2s[i])
			swap(y1s[i],y2s[i]);
	}
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];
	for(int j=0;j<m;++j)
	{
		for(int i=0;i<n;++i)
		{
			if(x1s[j]<=px[i]&&px[i]<=x2s[j]&&y1s[j]<=py[i]&&py[i]<=y2s[j])
			{
				++cnt[i];
				last[i]=j+1;
			}
		}
	}
	for(int i=0;i<n;++i)
	{
		if(cnt[i]==0)
			cout<<"NO\n";
		else
			cout<<"YES "<<cnt[i]<<' '<<last[i]<<'\n';
	}
	return 0;
}
