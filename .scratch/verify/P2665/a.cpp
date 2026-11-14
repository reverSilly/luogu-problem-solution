// 题目：P2665 [USACO08FEB] Game of Lines S
// 思路：每个方向（斜率）至多画一条、也必能画一条 ⇒ 答案 = 不同方向数。
//       方向用既约分数 (dy,dx) 规范化：dx>0、gcd=1；竖直线统一记 (1,0)。
//       枚举所有无序点对，set 去重计数。O(N^2 log N)。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,x[mxn],y[mxn];
set<pair<int,int>>S;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			int dx=x[j]-x[i],dy=y[j]-y[i];
			if(!dx)
			{
				S.emplace(1,0);		// 竖直线：所有竖直线互相平行，只算一个方向
				continue;
			}
			int g=gcd(abs(dx),abs(dy));
			dx/=g;dy/=g;
			if(dx<0)
				dx=-dx,dy=-dy;		// 分母规范化为正：(1,2) 与 (-1,-2) 同向
			S.emplace(dy,dx);
		}
	cout<<S.size()<<'\n';
	return 0;
}