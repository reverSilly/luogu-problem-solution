#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};
constexpr int mxh{10005};
constexpr int inf{1145141919};

int n;
int a[mxn];
bool can[mxh];                    // 当前城堡的子集和可达集合(穷举得到)
bool ok[mxh];                     // 所有城堡可达集合的交集

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	int lim{inf};
	fill_n(&ok[0],mxh,true);
	for(int i=0,x,m,sum;i<n;++i)
	{
		m=0;sum=0;
		while(cin>>x&&x!=-1){a[m++]=x;sum+=x;}
		lim=min(lim,sum);
		fill_n(&can[0],mxh,false);
		for(int mask=0;mask<(1<<m);++mask)   // 穷举每一块留/删,单座 m<=20
		{
			int h=0;
			for(int j=0;j<m;++j)
				if(mask>>j&1)
					h+=a[j];
			can[h]=true;
		}
		for(int h=0;h<=sum;++h)
			ok[h]=(ok[h]&&can[h]);
	}
	for(int h=lim;h>=0;--h)
		if(ok[h])
		{
			cout<<h<<'\n';
			return 0;
		}
	cout<<0<<'\n';
	return 0;
}
