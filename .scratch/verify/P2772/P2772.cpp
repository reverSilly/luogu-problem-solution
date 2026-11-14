// 题目：P2772 寻找平面上的极大点
// 极大点 = Pareto 前沿：不被任何其它点 (a>=x,b>=y) 支配
// 按 x 降序、y 降序排序后扫描，维护已扫最大 y；y>maxy 即极大
// 极大点 x 互异且 y 随 x 增而严格减；收集为 x 降序，逆序输出即 x 升序
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{500005};

int n,cnt;
struct point{int x,y;}p[mxn];
int ansx[mxn],ansy[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>p[i].x>>p[i].y;
	sort(p,p+n,[](const point&a,const point&b){
		if(a.x!=b.x)return a.x>b.x;
		return a.y>b.y;
	});
	int mxy=-1;
	for(int i=0;i<n;++i)
		if(p[i].y>mxy)
		{
			ansx[cnt]=p[i].x;ansy[cnt]=p[i].y;++cnt;
			mxy=p[i].y;
		}
	for(int i=cnt-1;i>=0;--i)
	{
		cout<<'('<<ansx[i]<<','<<ansy[i]<<')';
		if(i)cout<<',';
	}
	cout<<'\n';
	return 0;
}