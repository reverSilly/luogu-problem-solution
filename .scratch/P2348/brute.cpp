#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n,k,m,p;
	cin>>n>>k>>m>>p;
	vector<pair<string,string>>d(k);
	for(auto&[a,b]:d)
		cin>>a>>b;
	if(k<4*n)
	{
		cout<<"Error:cards not enough\n";
		return 0;
	}
	for(int t=0;t<m;++t)
	{
		int h=k/2;
		vector<pair<string,string>>nd;
		nd.reserve(k);
		for(int i=0;i<h;++i)
		{
			nd.push_back(d[h+i]);
			nd.push_back(d[i]);
		}
		if(k&1)
			nd.push_back(d[k-1]);
		d=nd;
	}
	// 真正按轮次发牌：前 4n 张依次给 1..n 号，每人 4 张
	vector<vector<pair<string,string>>>hand(n);
	for(int i=0;i<4*n;++i)
		hand[i%n].push_back(d[i]);
	for(auto&[a,b]:hand[p-1])
		cout<<a<<' '<<b<<'\n';
	return 0;
}
