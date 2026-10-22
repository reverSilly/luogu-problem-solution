#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{55};

int n,m,k,t;
//bomb[i]：第 i 个候选炸弹位置
int bx[mxn],by[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>t;
	for(int i=1;i<=k;++i)
		cin>>bx[i]>>by[i];

	i64 ans=0;
	for(int i=1;i<=k;++i)
	{
		i64 cnt=0;
		//只枚举炸弹周围的 (2t+1)^2 个格子
		for(int dx=-t;dx<=t;++dx)
		{
			int x=bx[i]+dx;
			if(x<1||x>n)
				continue;
			for(int dy=-t;dy<=t;++dy)
			{
				int y=by[i]+dy;
				if(y<1||y>m)
					continue;
				if(dx*dx+dy*dy<=t*t)
					++cnt;
			}
		}
		ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
