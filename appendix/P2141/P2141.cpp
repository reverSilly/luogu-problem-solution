#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int mxv{20005};

int a[mxn];
bool mark[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			int s=a[i]+a[j];
			//加数与被加数必须是集合中两个不同的数
			if(s<=mxv-5)
				mark[s]=true;
		}
	int ans=0;
	for(int i=0;i<n;++i)
		if(mark[a[i]])
			++ans;
	cout<<ans<<'\n';
	return 0;
}
