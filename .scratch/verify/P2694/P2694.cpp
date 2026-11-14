#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};

struct coin{int x,y;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int g;
	cin>>g;
	while(g--)
	{
		int n;
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i].x>>a[i].y;
		sort(a,a+n,[](coin p,coin q){return p.y!=q.y?p.y<q.y:p.x<q.x;});
		int px=0,py=0;
		bool ok=true;
		for(int i=0;i<n;++i)
		{
			if(abs(a[i].x-px)>a[i].y-py)
			{
				ok=false;
				break;
			}
			px=a[i].x;
			py=a[i].y;
		}
		cout<<(ok?"Abletocatch\n":"Notabletocatch\n");
	}
	return 0;
}