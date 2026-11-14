// 题目：P2705 小球
// 设 x=红球放蓝盒的个数，守恒 ⇒ 蓝球放红盒也恰是 x 个
// 得分=RC+BD+x(2E-C-D)，关于 x 线性 ⇒ 端点最优
#include<bits/stdc++.h>
using namespace std;

int R,B,C,D,E;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>R>>B>>C>>D>>E;
	int gain=2*E-C-D;	// 每多凑一对"交叉放置"的净收益
	cout<<R*C+B*D+max(0,min(R,B)*gain)<<'\n';
	return 0;
}