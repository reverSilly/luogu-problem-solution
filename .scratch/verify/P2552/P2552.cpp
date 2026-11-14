// 题目：P2552 [AHOI2001] 团体操队形
// 统一先按「按行排列」算出 (x=行, y=列)，按列排列时转置 swap(x,y)
// 连续队形：行优先填充，x=m/r+1, y=m%r+1（m 转 0-based）
// 梅花桩：奇数行占 a=(r+1)/2 个位置(奇数列)，偶数行占 r/2 个(偶数列)，每两行恰好 r 人
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int k;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k;
	for(int i=0;i<k;++i)
	{
		i64 n,m,r,x,y;
		int tp,dir;
		cin>>n>>tp>>dir>>r>>m;
		--m;					// 0-based，n 只用于约束 m，位置与 n 无关
		if(tp==1)				// 连续队形
		{
			x=m/r+1;
			y=m%r+1;
		}
		else					// 梅花桩队形
		{
			i64 a=(r+1)/2,p=m/r,q=m%r;
			if(q<a)				// 落在奇数行：第 p 对的第 q 个奇数列
			{
				x=2*p+1;
				y=2*q+1;
			}
			else				// 落在偶数行：第 q-a 个偶数列
			{
				x=2*p+2;
				y=2*(q-a)+2;
			}
		}
		if(dir==2)
			swap(x,y);			// 按列排列 = 按行排列的转置
		cout<<x<<' '<<y<<(i+1==k?'\n':' ');
	}
	return 0;
}