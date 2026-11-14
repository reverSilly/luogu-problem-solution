#include<bits/stdc++.h>
using namespace std;

int n,m,sa,sb;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,v;i<n;++i)
	{
		cin>>v;
		if(v)sa+=v;	// 打中：自己加分
		else sb+=4;	// 没打中：对方加 4
	}
	for(int i=0,v;i<m;++i)
	{
		cin>>v;
		if(v)sb+=v;
		else sa+=4;
	}
	cout<<sa<<' '<<sb<<'\n';
	return 0;
}