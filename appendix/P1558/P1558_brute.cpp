#include<bits/stdc++.h>
using namespace std;

int main()
{
	int L,T,O;
	cin>>L>>T>>O;
	vector<int> col(L,0);
	for(int i=0;i<O;++i)
	{
		char op;
		int a,b;
		cin>>op>>a>>b;
		if(a>b) swap(a,b);
		--a;--b;
		if(op=='C')
		{
			int c;
			cin>>c;
			--c;
			for(int j=a;j<=b;++j)
				col[j]=c;
		}
		else
		{
			int mask=0;
			for(int j=a;j<=b;++j)
				mask|=(1<<col[j]);
			cout<<__builtin_popcount(mask)<<'\n';
		}
	}
	return 0;
}
