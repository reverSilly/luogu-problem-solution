#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 w0,w;
int m;

bool check(double x)
{
	double bal=w0;
	for(int i=0;i<m;++i)
		bal=bal*(1+x)-w;
	return bal<=0;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>w0>>w>>m;
	double l=0,r=5;
	for(int i=0;i<100;++i)
	{
		double mid=(l+r)/2;
		if(check(mid))
			l=mid;
		else
			r=mid;
	}
	printf("%.1f\n",l*100);
	return 0;
}
