// 题目：P2038 无线网络发射器选址 —— 暴力：把每个公共场所的贡献铺到它能覆盖的所有路口上
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};
constexpr int sz{129};

int d,n;
int px[mxn],py[mxn];
i64 pk[mxn];
i64 g[sz][sz];	// g[x][y]: 在 (x,y) 装发射器能覆盖的公共场所总数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>d;
	cin>>n;
	for(int i=0;i<n;++i) cin>>px[i]>>py[i]>>pk[i];

	// 每个公共场所向它覆盖范围内的所有路口贡献 pk[i]
	for(int i=0;i<n;++i)
		for(int x=px[i]-d;x<=px[i]+d;++x)
			for(int y=py[i]-d;y<=py[i]+d;++y)
				if(x>=0&&x<sz&&y>=0&&y<sz)
					g[x][y]+=pk[i];

	i64 best=-1;
	int cnt=0;
	for(int x=0;x<sz;++x)
		for(int y=0;y<sz;++y)
			if(g[x][y]>best) best=g[x][y],cnt=1;
			else if(g[x][y]==best) ++cnt;
	cout<<cnt<<' '<<best<<'\n';
	return 0;
}
