//
// 题目：P1610 鸿山洞的灯
// https://www.luogu.com.cn/problem/P1610
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};                 // n<=1e5

int n,ans,lst;                             // lst: 上一盏保留下来的灯
i64 dist,p[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>dist;
	for(int i=0;i<n;++i)
		cin>>p[i];
	sort(p,p+n);

	for(int i=1;i<n-1;++i)                 // 第一盏与最后一盏必须亮
		if(p[i+1]-p[lst]<=dist)            // 关掉 i 后 lst 与 i+1 仍然够近
			++ans;
		else
			lst=i;                         // 关不掉, i 成为新的"最近亮灯"
	cout<<ans<<'\n';
	return 0;
}
