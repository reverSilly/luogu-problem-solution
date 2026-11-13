#include<bits/stdc++.h>
using namespace std;
int main()
{
	static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	int L=1+rng()%2,R=1+rng()%2,C=1+rng()%3;
	if(L*R*C<2){C=2;}
	int tot=L*R*C;
	vector<char>v(tot);
	for(int i=0;i<tot;++i)v[i]=(rng()%100<30)?'#':'.';
	int s=rng()%tot,e=rng()%tot;
	while(e==s)e=rng()%tot;
	v[s]='S';v[e]='E';
	printf("%d %d %d\n",L,R,C);
	for(int i=0;i<L;++i)
	{
		for(int j=0;j<R;++j)
		{
			for(int k=0;k<C;++k)
				putchar(v[(i*R+j)*C+k]);
			putchar('\n');
		}
	}
	return 0;
}
