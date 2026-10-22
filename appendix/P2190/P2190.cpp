// 题目：P2190 小Z的车厢
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

i64 diff_[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	// 站点 i 的出站方向（顺时针）记为第 i 段（i=n 时为 n->1 那一段）
	for(int i=1;i<=m;++i)
	{
		int x,y,z;
		cin>>x>>y>>z;
		if(x<y)
		{
			// 经过第 x,x+1,...,y-1 段
			diff_[x]+=z;
			diff_[y]-=z;
		}
		else
		{
			// 经过第 x,...,n 段与第 1,...,y-1 段，跨越 1->n
			diff_[x]+=z;
			diff_[n+1]-=z;
			diff_[1]+=z;
			diff_[y]-=z;
		}
	}
	i64 mx=0,cur=0;
	for(int i=1;i<=n;++i)
	{
		cur+=diff_[i];
		mx=max(mx,cur);
	}
	// 每节车厢 36 人
	cout<<(mx+35)/36<<'\n';
	return 0;
}
