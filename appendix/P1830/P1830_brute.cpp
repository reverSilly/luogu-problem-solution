//
// 题目：P1830 轰炸III
// https://www.luogu.com.cn/problem/P1830
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxb{2005};

int x1s[mxb],y1s[mxb],x2s[mxb],y2s[mxb];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m,x,y;
	cin>>n>>m>>x>>y;
	if(n<1||m<1)
		return 0;
	for(int k=0;k<x;++k)
	{
		cin>>x1s[k]>>y1s[k]>>x2s[k]>>y2s[k];
		if(x1s[k]>x2s[k])
			swap(x1s[k],x2s[k]);
		if(y1s[k]>y2s[k])
			swap(y1s[k],y2s[k]);
	}
	for(int t=0;t<y;++t)
	{
		int a,b,cnt=0,lst=0;
		cin>>a>>b;
		for(int k=0;k<x;++k)
			if(x1s[k]<=a&&a<=x2s[k]&&y1s[k]<=b&&b<=y2s[k])
			{
				++cnt;
				lst=k+1;
			}
		if(cnt==0)
			cout<<"N\n";
		else
			cout<<"Y "<<cnt<<' '<<lst<<'\n';
	}
	return 0;
}
