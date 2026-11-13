// 题目：P2449 [SDOI2005] 矩形
// 思路：块 = 连通分量。两矩形交为 [max x1,min x2]x[max y1,min y2]，
//       dx<0||dy<0 分离；dx==0&&dy==0 贴角（不算）；其余（含贴边）合并。
//       n<=7000，O(n^2) 并查集足够；按 x1 排序后内层可 break 剪枝。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{7005};

int n,f[mxn];
struct rect{int x1,y1,x2,y2;}r[mxn];

int getrt(int x)
{
	if(f[x]==x)return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)cin>>r[i].x1>>r[i].y1>>r[i].x2>>r[i].y2;
	sort(r,r+n,[](const rect&a,const rect&b){return a.x1<b.x1;});
	iota(f,f+n,0);
	int ans=n;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n&&r[j].x1<=r[i].x2;++j)//x1 升序，越过右边界即可 break
		{
			int dx=min(r[i].x2,r[j].x2)-max(r[i].x1,r[j].x1);
			int dy=min(r[i].y2,r[j].y2)-max(r[i].y1,r[j].y1);
			if(dx<0||dy<0||(dx==0&&dy==0))continue;//不相交，或只交于一个角
			int u=getrt(i),v=getrt(j);
			if(u!=v){f[u]=v;--ans;}
		}
	cout<<ans<<'\n';
	return 0;
}
