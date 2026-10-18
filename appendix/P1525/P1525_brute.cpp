//
// 题目：P1525 [NOIP 2010 提高组] 关押罪犯
// 暴力基准: 枚举 2^(n-1) 种将 n 名罪犯分入两座监狱的 0/1 划分方案, 严格按定义求同狱最大冲突的最小值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{1005};
constexpr int inf{1145141919};

int n,m;
int eu[mxm],ev[mxm],ew[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<m;++i)
	{
		cin>>eu[i]>>ev[i]>>ew[i];
		--eu[i];--ev[i];
	}

	int ans=inf;
	int lim=1<<max(0,n-1);        // 固定 0 号在监狱 0, 对称减半
	for(int mask=0;mask<lim;++mask)
	{
		int mx=0;
		for(int i=0;i<m;++i)
		{
			int c1=(mask>>eu[i])&1;
			int c2=(mask>>ev[i])&1;
			if(c1==c2)
				mx=max(mx,ew[i]);
		}
		ans=min(ans,mx);
	}
	cout<<ans<<'\n';
	return 0;
}
