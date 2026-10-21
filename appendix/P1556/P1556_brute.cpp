#include<bits/stdc++.h>
using namespace std;

int n;
int x[15],y[15];

int get_dir(int u,int v)
{
	if(x[u]==x[v]&&y[u]==y[v]) return -1;
	if(x[u]==x[v]) return y[v]>y[u]?0:1;
	if(y[u]==y[v]) return x[v]>x[u]?2:3;
	return -1;
}

int main()
{
	cin>>n;
	x[0]=y[0]=0;
	for(int i=1;i<=n;++i)
		cin>>x[i]>>y[i];
	vector<int> p(n);
	iota(p.begin(),p.end(),1);
	long long ans=0;
	do
	{
		int prev=0,last_d=-1;
		bool ok=true;
		for(int u:p)
		{
			int d=get_dir(prev,u);
			if(d==-1||(prev!=0&&d==last_d))
			{
				ok=false;
				break;
			}
			last_d=d;
			prev=u;
		}
		if(ok)
		{
			int d=get_dir(prev,0);
			if(d!=-1&&d!=last_d)
				++ans;
		}
	}while(next_permutation(p.begin(),p.end()));
	cout<<ans<<'\n';
	return 0;
}
