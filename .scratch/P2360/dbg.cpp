#include<bits/stdc++.h>
using namespace std;
int L,R,C;
char mp[32][32][32];
int main()
{
	while(cin>>L>>R>>C)
	{
		if(L==0||R==0||C==0)break;
		for(int i=0;i<L;++i)
			for(int j=0;j<R;++j)
				for(int k=0;k<C;++k)
					cin>>mp[i][j][k];
		cerr<<"L="<<L<<" R="<<R<<" C="<<C<<"\n";
		for(int i=0;i<L;++i)
		{
			for(int j=0;j<R;++j)
			{
				for(int k=0;k<C;++k)
					cerr<<mp[i][j][k];
				cerr<<"\n";
			}
			cerr<<"\n";
		}
	}
}
