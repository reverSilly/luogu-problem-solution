#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxt{1000005};

int n,m;
i64 ps[mxt],pk[mxt];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	int ts=0,tk=0;
	ps[0]=0;
	for(int i=0,v,d;i<n;++i){cin>>v>>d; for(int e=0;e<d;++e){++ts;ps[ts]=ps[ts-1]+v;}}
	pk[0]=0;
	for(int i=0,v,d;i<m;++i){cin>>v>>d; for(int e=0;e<d;++e){++tk;pk[tk]=pk[tk-1]+v;}}
	int T=min(ts,tk);
	vector<int> seq;             // 依次记录非平局时刻的领先者符号
	for(int t=1;t<=T;++t)
	{
		i64 d=ps[t]-pk[t];
		if(d>0)
			seq.push_back(1);
		else
			if(d<0)
				seq.push_back(-1);
	}
	vector<int> c;               // 合并连续相同领先者
	for(int v:seq)
		if(c.empty()||c.back()!=v)
			c.push_back(v);
	int ans=c.empty()?0:(int)c.size()-1;   // 相邻不同即一次领先顺序变化
	cout<<ans<<'\n';
	return 0;
}
