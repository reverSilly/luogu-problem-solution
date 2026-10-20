//
// 题目：P1563 [NOIP 2016 提高组] 玩具谜题
// https://www.luogu.com.cn/problem/P1563
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // n,m<=1e5 (+5 松弛)

int n,m;
int fcg[mxn];                     // 朝向: 0 面朝圈内, 1 面朝圈外
string nm[mxn];                   // 职业名

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>fcg[i]>>nm[i];
	int pos{0};                   // 下标增大的方向 = 逆时针
	for(int i=0,a,s;i<m;++i)
	{
		cin>>a>>s;
		if((a^fcg[pos])==1)       // 左/右 与 朝向 异或为 1 => 逆时针(下标+)
			pos=(pos+s)%n;
		else
			pos=(pos-s+n)%n;
	}
	cout<<nm[pos]<<'\n';
	return 0;
}
