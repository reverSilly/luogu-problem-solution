// 题目：P2785 物理1（phsic1）- 磁通量
// 磁通量 = B × 多边形面积；面积用鞋带公式（有向面积取绝对值）。
// 平移到首点原点：鞋带和不变（平移不变性），叉积量级只随多边形自身尺寸，精度更稳。
// 流式读入：只留上一点（相对坐标），O(1) 额外空间，N 再大也不怕。
#include<bits/stdc++.h>
using namespace std;

int n;
long double B;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>B;
	long double x0,y0,px=0,py=0,x,y,s=0;
	cin>>x0>>y0;
	for(int i=1;i<n;++i)
	{
		cin>>x>>y;
		x-=x0;y-=y0;	// 平移不改变鞋带和；首点变为原点
		s+=px*y-x*py;	// 有向叉积累加
		px=x;py=y;
	}
	// 闭合边 cross(末点,首点)=cross(末点,原点)=0，无需显式累加
	cout<<fixed<<setprecision(4)<<fabsl(s)/2*B<<'\n';
	return 0;
}
