// 题目：P2095 营养膳食
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,m,k;
int lim[mxn];
struct food{int a,b;}f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=1;i<=k;++i)
		cin>>lim[i];
	for(int i=0;i<n;++i)
		cin>>f[i].a>>f[i].b;
	sort(f,f+n,[](food x,food y){
		return x.a>y.a;
	});
	int ans=0;
	for(int i=0;i<n&&m>0;++i)
	{
		if(lim[f[i].b]>0)
		{
			lim[f[i].b]--;
			--m;
			ans+=f[i].a;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
