// P2735 暴力：逐格点判断是否严格在三角形内部（三条边同侧法）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int main()
{
	i64 n,m,p;cin>>n>>m>>p;
	i64 cnt=0;
	i64 X=max(n,p);
	for(i64 x=0;x<=X;++x)
		for(i64 y=0;y<=m;++y)
		{
			// 严格内部：y>0 且 mx-ny>0 且 mx+(p-n)y<mp
			if(y>0&&m*x-n*y>0&&m*x+(p-n)*y<m*p)
				++cnt;
		}
	cout<<cnt<<'\n';
	return 0;
}