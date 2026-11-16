// 题目：P2694 接金币
// 思路：金币 i 只在 t=y_i 落到 y=0，故必须时刻 y_i 站在 x_i。
//       按 (y,x) 排序，从 (0,0) 起逐段检查 |Δx|≤Δy（速度≤1，可停⇒无奇偶约束）。
//       同时刻不同位置 / y=0 非原点 都被该检查自动判否。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};

struct coin{int x,y;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int g;
	cin>>g;
	while(g--)
	{
		int n;
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i].x>>a[i].y;
		sort(a,a+n,[](coin p,coin q){return p.y!=q.y?p.y<q.y:p.x<q.x;});
		int px=0,py=0;	// 虚拟起点 (0,0)
		bool ok=true;
		for(int i=0;i<n;++i)
		{
			if(abs(a[i].x-px)>a[i].y-py)	// 该段超速 ⇒ 不可行
			{
				ok=false;
				break;
			}
			px=a[i].x;
			py=a[i].y;
		}
		cout<<(ok?"Abletocatch\n":"Notabletocatch\n");
	}
	return 0;
}
