// 暴力基准:对每个 a_i 线性扫描 b 判断存在,O(n*m)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{315};       // 暴力仅跑 n,m<=300

int n,m;
int a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
		cin>>b[i];

	bool first=true;
	for(int i=0;i<n;++i)
	{
		bool found=false;
		for(int j=0;j<m;++j)
			if(b[j]==a[i])
			{
				found=true;
				break;
			}
		if(found)
		{
			if(!first)
				cout<<' ';
			cout<<a[i];
			first=false;
		}
	}
	cout<<'\n';
	return 0;
}
