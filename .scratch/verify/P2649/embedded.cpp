// 题目：P2649 游戏预言
// 思路：对手用一张更大的补集牌压住 John 的一张牌；最多压住 beat 张（最大匹配）。
//       答案 = n - beat。m-1 个对手只通过补集大小 n(m-1) 起作用，filler 总能凑齐。
//       贪心：John 牌升序、补集升序，每张 John 牌配最小的能压过它的补集牌。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int m,n,a[mxn],b[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		vis[a[i]]=true;
	}
	int cnt=0;
	for(int i=1;i<=n*m;++i)		// 补集：不在 John 手里的牌
		if(!vis[i])
			b[cnt++]=i;
	sort(a,a+n);
	sort(b,b+cnt);
	int j=0,beat=0;
	for(int i=0;i<n;++i)		// 贪心：每张 John 的牌配一张最小的能压过它的牌
	{
		while(j<cnt&&b[j]<=a[i])
			++j;
		if(j<cnt)
		{
			++beat;
			++j;
		}
	}
	cout<<n-beat<<'\n';			// 压不掉的牌数 = 最少赢的轮数
	return 0;
}