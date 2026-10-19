// 题目：P1548 [NOIP 1997 普及组] 棋盘问题
// 暴力基准：枚举所有横向/纵向网格线选出的矩形,逐个判定是否正方形
// 仅用于小规模对拍,不能提交
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	i64 sq=0,rect=0;
	for(int t=0;t<n;++t)                          // 上边界
		for(int b=t+1;b<=n;++b)                   // 下边界
			for(int l=0;l<m;++l)                  // 左边界
				for(int r=l+1;r<=m;++r)           // 右边界
				{
					int h=b-t,w=r-l;
					if(h==w)
						++sq;
					else
						++rect;
				}
	cout<<sq<<' '<<rect<<'\n';
	return 0;
}
