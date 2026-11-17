// 题目：P2945 [USACO09MAR] Sand Castle S
// 排序后按序配对最优（交换论证：代价函数关于 (m,b) 满足四边形不等式）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25005};

int n,x,y,m[mxn],b[mxn];
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>x>>y;
	for(int i=0;i<n;++i)
		cin>>m[i]>>b[i];
	sort(m,m+n);sort(b,b+n);
	for(int i=0;i<n;++i)
		ans+=b[i]>m[i]?(i64)x*(b[i]-m[i]):(i64)y*(m[i]-b[i]);
	cout<<ans<<'\n';
	return 0;
}
