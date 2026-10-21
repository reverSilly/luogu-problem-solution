#include<bits/stdc++.h>
using namespace std;

constexpr int mxv=200000;
int d[mxv+5],best_super[mxv+5];

int main()
{
	for(int i=1;i<=mxv;++i)
		for(int j=i;j<=mxv;j+=i)
			++d[j];
	int max_d=0,cur_s=1;
	for(int i=1;i<=mxv;++i)
	{
		if(d[i]>max_d)
		{
			max_d=d[i];
			cur_s=i;
		}
		best_super[i]=cur_s;
	}
	int m;
	cin>>m;
	while(m--)
	{
		int a;
		cin>>a;
		cout<<best_super[a]<<'\n';
	}
	return 0;
}
