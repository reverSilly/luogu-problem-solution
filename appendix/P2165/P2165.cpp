#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int len[mxn],pos[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int sum=0;
	for(int i=0;i<n;++i)
	{
		cin>>len[i];
		sum+=len[i];
	}
	pos[0]=0;
	for(int i=1;i<n;++i)
		pos[i]=pos[i-1]+len[i-1];

	//内接矩形的对角线必为直径，统计直径条数再两两配对
	int cnt=0;
	if(sum%2==0)
		for(int i=0;i<n;++i)
			for(int j=i+1;j<n;++j)
				if(pos[j]-pos[i]==sum/2)
					++cnt;
	cout<<cnt*(cnt-1)/2<<'\n';
	return 0;
}
