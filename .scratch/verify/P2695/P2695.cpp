#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005};

int n,m,a[mxn],z[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
		cin>>z[i];
	sort(a,a+n);
	sort(z,z+m);
	i64 ans=0;
	int j=0;
	for(int i=0;i<n;++i)
	{
		while(j<m&&z[j]<a[i])
			++j;
		if(j==m)
		{
			cout<<"you died!\n";
			return 0;
		}
		ans+=z[j];
		++j;
	}
	cout<<ans<<'\n';
	return 0;
}