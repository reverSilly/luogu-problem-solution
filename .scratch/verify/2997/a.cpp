// 题目：P2997 [USACO10NOV] Banner S
// 按位移分类：内部无整点 ⟺ gcd(dx,dy)=1；长度用平方整数比较，零浮点
// dx>0 且 dy>0 时正负两种斜率各贡献 (W+1-dx)*(H+1-dy) 个平移位置
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int w,h,l1,l2;

int gcd(int a,int b){return b?gcd(b,a%b):a;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>w>>h>>l1>>l2;
	i64 lo=(i64)l1*l1,hi=(i64)l2*l2,ans=0;
	for(int dx=0;dx<=w;++dx)
		for(int dy=(dx==0);dy<=h;++dy)	// dx=0 时 dy 从 1 起，排除 (0,0)
		{
			i64 d2=(i64)dx*dx+(i64)dy*dy;
			if(d2<lo||d2>hi||gcd(dx,dy)!=1)
				continue;
			i64 cnt=(i64)(w+1-dx)*(h+1-dy);
			ans+=(dx>0&&dy>0?2:1)*cnt;	// 水平/竖直无序对无方向，不乘 2
		}
	cout<<ans<<'\n';
	return 0;
}
