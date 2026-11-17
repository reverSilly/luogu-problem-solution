// 题目：P2909 [USACO08OPEN] Cow Cars S
// 每头在高速上的牛速度 = max(S-D*同车道前方牛数,0)，须 >=L。
// 位置 (车道,深度k) 是一个门槛为 L+D*k 的"槽位"；按门槛从小到大、
// 牛速从小到大贪心匹配：排序后扫一遍，s[i]>=L+D*(ans/M) 就收。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n,m,d,l,s[mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>d>>l;
	for(int i=0;i<n;++i)
		cin>>s[i];
	sort(s,s+n);
	for(int i=0;i<n;++i)
		if(s[i]>=l+d*(ans/m))	// ans/m = 当前最浅空槽的深度；d*(ans/m)<=2.5e8 不爆 int
			++ans;
	cout<<ans<<'\n';
	return 0;
}
