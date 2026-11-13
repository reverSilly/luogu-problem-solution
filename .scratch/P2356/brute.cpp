#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n;
	cin>>n;
	vector<vector<int>>a(n,vector<int>(n));
	for(auto&r:a)
		for(int&x:r)
			cin>>x;
	int ans=-1;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			if(a[i][j]==0)
			{
				int s=0;
				for(int c=0;c<n;++c)
					s+=a[i][c];
				for(int r=0;r<n;++r)
					s+=a[r][j];
				// (i,j) 为 0，重复算两次也不影响
				ans=max(ans,s);
			}
	if(ans<0)
		cout<<"Bad Game!\n";
	else
		cout<<ans<<'\n';
	return 0;
}
