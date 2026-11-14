#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m;
string word[mxn];
bool same[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	fill_n(same,n,true);
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			string w;
			cin>>w;
			if(i==0)
				word[j]=w;
			else if(w!=word[j])
				same[j]=false;
		}
	for(int j=0;j<n;++j)
	{
		if(j)
			cout<<' ';
		cout<<(same[j]?word[j]:string("*"));
	}
	cout<<'\n';
	return 0;
}