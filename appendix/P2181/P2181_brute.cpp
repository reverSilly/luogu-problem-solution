// P2181 暴力：枚举所有对角线两两组合，判断是否在内部相交，统计交点数
#include<bits/stdc++.h>
using namespace std;

int n;

struct Seg
{
	int a,b;
};

// 四个端点互不相同，且在圆周上严格交替出现时两条线段相交
bool cross(Seg s1,Seg s2)
{
	if(s1.a==s2.a||s1.a==s2.b||s1.b==s2.a||s1.b==s2.b)
		return false;
	int p[4]={s1.a,s1.b,s2.a,s2.b};
	sort(p,p+4);
	int idxA=-1,idxB=-1,idxC=-1,idxD=-1;
	for(int i=0;i<4;++i)
	{
		if(p[i]==s1.a)idxA=i;
		if(p[i]==s1.b)idxB=i;
		if(p[i]==s2.a)idxC=i;
		if(p[i]==s2.b)idxD=i;
	}
	int x1=min(idxA,idxB),x2=max(idxA,idxB);
	int y1=min(idxC,idxD),y2=max(idxC,idxD);
	return (x1<y1&&y1<x2&&x2<y2)||(y1<x1&&x1<y2&&y2<x2);
}

int main()
{
	scanf("%d",&n);
	vector<Seg> d;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			// 相邻顶点之间是边而非对角线
			if(j==i+1||(i==0&&j==n-1))
				continue;
			d.push_back({i,j});
		}
	long long ans=0;
	for(size_t i=0;i<d.size();++i)
		for(size_t j=i+1;j<d.size();++j)
			if(cross(d[i],d[j]))
				++ans;
	printf("%lld\n",ans);
	return 0;
}
