// 题目：P2434 [SDOI2005] 区间
// 闭区间并的最少不相交表示 = 连通分量；排序后扫描合并，判据 a<=R（带等号）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n;
struct seg{int a,b;}s[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>s[i].a>>s[i].b;
	sort(s,s+n,[](seg x,seg y){return x.a!=y.a?x.a<y.a:x.b<y.b;});
	int L=s[0].a,R=s[0].b;
	for(int i=1;i<n;++i)
	{
		if(s[i].a<=R)			// 闭区间：端点相接也算相交，必须带等号
			R=max(R,s[i].b);
		else
		{
			cout<<L<<' '<<R<<'\n';
			L=s[i].a;R=s[i].b;
		}
	}
	cout<<L<<' '<<R<<'\n';
	return 0;
}
