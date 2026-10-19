#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,k;
int cnt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	vector<int>a(n);
	iota(a.begin(),a.end(),1);
	do
	{
		int inv=0;
		for(int i=0;i<n;++i)
			for(int j=i+1;j<n;++j)
				if(a[i]>a[j])
					++inv;
		if(inv<mxn)
			++cnt[inv];
	}while(next_permutation(a.begin(),a.end()));
	cout<<(k<mxn?cnt[k]%10000:0)<<'\n';
	return 0;
}
