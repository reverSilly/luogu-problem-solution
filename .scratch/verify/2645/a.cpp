// 题目：P2645 斯诺克
// 思路：Background 的真实斯诺克规则全部作废，只按 Description 的简化规则：
//       打中分值 v(1~7) → 自己加 v；没打中(0) → 对方加 4；球不拿走 ⇒ 每轮独立。
//       贡献是纯累加，顺序无关，先扫 Ar 的 n 轮再扫 Be 的 m 轮即可。
#include<bits/stdc++.h>
using namespace std;

int n,m,sa,sb;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,v;i<n;++i)
	{
		cin>>v;
		if(v)sa+=v;	// Ar 打中：自己加分
		else sb+=4;	// Ar 没打中：对方(Be)加 4
	}
	for(int i=0,v;i<m;++i)
	{
		cin>>v;
		if(v)sb+=v;	// Be 打中：自己加分
		else sa+=4;	// Be 没打中：对方(Ar)加 4
	}
	cout<<sa<<' '<<sb<<'\n';
	return 0;
}
