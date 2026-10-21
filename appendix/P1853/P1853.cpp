//
// 题目：P1853 [NWERC 2004] 投资的最大效益
// https://www.luogu.com.cn/problem/P1853
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxc{50005};
constexpr int mxd{15};

int d,w[mxd],v[mxd],f[mxc];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 money;
	int yr;
	cin>>money>>yr>>d;
	for(int i=0;i<d;++i)
	{
		cin>>w[i]>>v[i];
		w[i]/=1000;
	}
	for(int i=0;i<d;++i)
	{
		for(int j=w[i];j<mxc;++j)
		{
			int nv=f[j-w[i]]+v[i];
			if(nv>f[j])
				f[j]=nv;
		}
	}
	for(int y=0;y<yr;++y)
	{
		int cap=(int)(money/1000);
		money+=f[cap];
	}
	cout<<money<<'\n';
	return 0;
}
