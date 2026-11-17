// 题目：P2772 寻找平面上的极大点
// 按 (x,y) 字典序升序排序，从右往左扫后缀最大 y；y 严格大于后缀最大值 ⟺ 极大点
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{500005};

struct point{int x,y;}a[mxn],res[mxn];
int n,cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].x>>a[i].y;
	sort(a,a+n,[](point p,point q){return p.x!=q.x?p.x<q.x:p.y<q.y;});
	int mxy=-114514;	// y>=0，哨兵低于一切合法坐标
	for(int i=n-1;i>=0;--i)
		if(a[i].y>mxy)	// 必须严格 >：x 更大而 y 相等也算支配
		{
			res[cnt++]=a[i];
			mxy=a[i].y;
		}
	for(int i=cnt-1;i>=0;--i)	// 从右往左收集，倒序输出即 x 升序
	{
		if(i!=cnt-1)
			cout<<',';
		cout<<'('<<res[i].x<<','<<res[i].y<<')';
	}
	cout<<'\n';
	return 0;
}
