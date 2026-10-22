// 题目：P2096 最佳旅游线路
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int m,n;
int col[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	//col 存每一列(林阴道之间的一小段旅游街)的最大值
	fill_n(col,n,-1145141919);
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			int x;cin>>x;
			col[j]=max(col[j],x);
		}

	//最大子段和:col 的连续若干列之和的最大值
	int cur=0,ans=-1145141919;
	for(int j=0;j<n;++j)
	{
		cur=max(col[j],cur+col[j]);
		ans=max(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
