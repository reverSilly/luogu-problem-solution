// 题目：P2096 最佳旅游线路 —— 暴力：逐格 DP，显式维护"当前在哪一行"
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int m,n;
int a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	const int neg=-1145141919;
	int pre[mxn],cur[mxn];
	for(int i=0;i<m;++i) pre[i]=neg;
	int ans=neg;
	for(int j=0;j<n;++j)
	{
		// 换行免费: 进入第 j 列时可以处于任意一行
		int best=neg;
		for(int i=0;i<m;++i) best=max(best,pre[i]);
		for(int i=0;i<m;++i)
		{
			cur[i]=a[i][j]+max(0,best);
			ans=max(ans,cur[i]);
		}
		for(int i=0;i<m;++i) pre[i]=cur[i];
	}
	cout<<ans<<'\n';
	return 0;
}
