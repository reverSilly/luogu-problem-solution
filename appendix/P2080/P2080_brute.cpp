#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{35};

int n,v;
int a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>v;
	for(int i=1;i<=n;++i)
		cin>>a[i]>>b[i];

	//暴力：枚举所有子集
	int ans=-1;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int A=0,B=0;
		for(int i=1;i<=n;++i)
		{
			if(mask>>(i-1)&1)
			{
				A+=a[i];
				B+=b[i];
			}
		}
		if(A+B<v)
			continue;
		int d=abs(A-B);
		if(ans==-1||d<ans)
			ans=d;
	}
	cout<<ans<<'\n';
	return 0;
}
