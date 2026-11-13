#include<bits/stdc++.h>
using namespace std;

int n;
multiset<int>S;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int typ=-1;
	long long ans=0;
	for(int i=0,a,b;i<n;++i)
	{
		cin>>a>>b;
		if(S.empty())
		{
			typ=a;
			S.insert(b);
			continue;
		}
		if(a==typ)
		{
			S.insert(b);
			continue;
		}
		auto it=S.lower_bound(b);
		if(it==S.end())
			--it;
		else if(it!=S.begin())
		{
			auto pre=prev(it);
			if(b-*pre<=*it-b)
				it=pre;
		}
		ans+=abs(b-*it);
		S.erase(it);
	}
	cout<<ans%1000000<<'\n';
	return 0;
}
