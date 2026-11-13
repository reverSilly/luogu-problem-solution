#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n;
int a[mxn][mxn],rs[mxn],cs[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			cin>>a[i][j];
			rs[i]+=a[i][j];
			cs[j]+=a[i][j];
		}
	int ans=-1;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(a[i][j]==0)
				ans=max(ans,rs[i]+cs[j]);
	if(ans<0)
		cout<<"Bad Game!\n";
	else
		cout<<ans<<'\n';
	return 0;
}
