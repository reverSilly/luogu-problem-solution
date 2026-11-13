#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(chrono::steady_clock::now().time_since_epoch().count());
	int n=rn()%12+1;
	cout<<n<<'\n';
	int mode=rn()%5;	//0: 全非零; 1: 全零; 其他: 混合
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			int x;
			if(mode==0)
				x=rn()%1000+1;
			else if(mode==1)
				x=0;
			else
				x=(rn()%2)?0:rn()%1000+1;
			cout<<x<<(j==n-1?'\n':' ');
		}
	}
	return 0;
}
