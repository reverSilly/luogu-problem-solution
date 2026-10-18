//
// 题目：P1516 青蛙的约会
// 暴力基准: 逐步模拟 t=0,1,...,L-1 检验两只青蛙在模 L 环上的位置是否重合(仅小 L 可用)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 x,y,m,n,L;
	if(!(cin>>x>>y>>m>>n>>L))
		return 0;

	i64 pa=x%L,pb=y%L;
	for(i64 t=0;t<L;++t)
	{
		if(pa==pb)
		{
			cout<<t<<'\n';
			return 0;
		}
		pa=(pa+m)%L;
		pb=(pb+n)%L;
	}
	cout<<"Impossible\n";
	return 0;
}
