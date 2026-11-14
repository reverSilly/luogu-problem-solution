#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	cin>>s;
	int n=s.size(),flips=0;
	for(int k=n-1;k>=0;--k)
	{
		int cur=(s[k]-'0')^(flips&1);
		if(cur==0)
			++flips;
	}
	cout<<flips<<'\n';
	return 0;
}