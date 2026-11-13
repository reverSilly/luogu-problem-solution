// 题目：P2356 弹珠游戏
// 思路：站在空格 (i,j) 的得分 = 第 i 行敌人分数和 + 第 j 列敌人分数和
//       （该格本身是 0，不会被重复计算）。预处理每行、每列的和后扫描所有空格。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n;
int a[mxn][mxn],rs[mxn],cs[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			cin>>a[i][j];
			rs[i]+=a[i][j];
			cs[j]+=a[i][j];
		}
	int ans=-1;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(a[i][j]==0)
				ans=max(ans,rs[i]+cs[j]);
	if(ans<0)
		cout<<"Bad Game!\n";
	else
		cout<<ans<<'\n';
	return 0;
}
