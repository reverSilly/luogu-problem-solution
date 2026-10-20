#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{35};

int t,m,x[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>t;
	while(t--)
	{
		cin>>m;
		for(int i=0;i<m;++i)
			cin>>x[i];
		int ans=0;
		for(int i=0;i<m;++i)
		{
			int others[mxn],k=0;
			for(int j=0;j<m;++j)
				if(j!=i)
					others[k++]=x[j];
			for(int mask=0;mask<(1<<k);++mask)   // 穷举其余元素的全部子集
			{
				int sum=0;
				for(int b=0;b<k;++b)
					if(mask>>b&1)
						sum+=others[b];
				if(sum==x[i])
					++ans;
			}
		}
		cout<<ans<<'\n';
	}
	return 0;
}
