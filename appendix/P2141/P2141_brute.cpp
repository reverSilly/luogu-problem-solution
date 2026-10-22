#include<bits/stdc++.h>
using namespace std;

//暴力：对每个元素，判断它能否表示成集合中另外两个不同数之和
constexpr int mxn{105};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int ans=0;
	for(int i=0;i<n;++i)
	{
		bool ok=false;
		for(int j=0;j<n&&!ok;++j)
		{
			if(j==i)continue;
			for(int k=j+1;k<n;++k)
			{
				if(k==i)continue;
				if(a[j]+a[k]==a[i])
				{
					ok=true;
					break;
				}
			}
		}
		if(ok)++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
