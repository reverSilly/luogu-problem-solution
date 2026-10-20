//
// 题目：P1556 [USACO12MAR] 幸福的路 Connect the Cows B ——朴素暴力基准
// https://www.luogu.com.cn/problem/P1556
// 结构独立于标程: next_permutation 枚举全部访问顺序, 逐个显式验证
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};
int n;
int x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	vector<int> p(n);
	iota(p.begin(),p.end(),0);
	i64 ans=0;
	do
	{
		int px=0,py=0;            // 上一个转折点, 初始为原点
		int inx=0,iny=0;
		bool has=false,ok=true;
		for(int k=0;k<n;++k)
		{
			int cx=x[p[k]],cy=y[p[k]];
			if(px!=cx&&py!=cy){ok=false;break;}
			int ox=(cx>px)-(cx<px),oy=(cy>py)-(cy<py);
			if(has&&ox==inx&&oy==iny){ok=false;break;}   // 直行=没转弯
			inx=ox;iny=oy;has=true;
			px=cx;py=cy;
		}
		if(ok)
		{
			if(px!=0&&py!=0)
				ok=false;                                // 末点回不到原点
			else
			{
				int ox=(0>px)-(0<px),oy=(0>py)-(0<py);
				if(ox==inx&&oy==iny)
					ok=false;                            // 末年未转弯
			}
		}
		if(ok)
			++ans;
	}while(next_permutation(p.begin(),p.end()));
	cout<<ans<<'\n';
	return 0;
}
