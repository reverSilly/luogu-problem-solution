//
// 题目：P1830 轰炸III
// https://www.luogu.com.cn/problem/P1830
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int tim[mxn][mxn],last[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m,x,y;
	cin>>n>>m>>x>>y;
	if(n<1||m<1)
		return 0;
	for(int k=1;k<=x;++k)
	{
		int x1,y1,x2,y2;
		cin>>x1>>y1>>x2>>y2;
		if(x1>x2)
			swap(x1,x2);
		if(y1>y2)
			swap(y1,y2);
		for(int i=x1;i<=x2;++i)
			for(int j=y1;j<=y2;++j)
			{
				++tim[i][j];
				last[i][j]=k;
			}
	}
	for(int k=0;k<y;++k)
	{
		int a,b;
		cin>>a>>b;
		if(tim[a][b]==0)
			cout<<"N\n";
		else
			cout<<"Y "<<tim[a][b]<<' '<<last[a][b]<<'\n';
	}
	return 0;
}
