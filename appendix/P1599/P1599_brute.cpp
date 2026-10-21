//
// P1599 暴力: 状态 (位置, 现金, 已结算集合) 上的 Dijkstra, 每步代价 1
// 现金上限取所有正数之和; n<=7, |D|<=6 => 状态数 <= 8*43*128
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10};
constexpr int mxc{200};

int n,d[mxn];
int dist_[mxn][mxc][1<<8];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int cap=0;
	for(int i=1;i<=n;++i)
	{
		cin>>d[i];
		if(d[i]>0)
			cap+=d[i];
	}
	if(cap>=mxc)
	{
		cout<<"TOOBIG\n";
		return 0;
	}

	memset(dist_,0x3f,sizeof(dist_));
	int full=(1<<n)-1;
	dist_[0][0][0]=0;
	using st=tuple<int,int,int,int>;             // d, pos, cash, mask
	priority_queue<st,vector<st>,greater<st>>Q;
	Q.push({0,0,0,0});
	while(!Q.empty())
	{
		auto[cw,p,c,mask]=Q.top();
		Q.pop();
		if(cw!=dist_[p][c][mask])
			continue;
		if(p==n&&mask==full)
		{
			cout<<cw<<'\n';
			return 0;
		}
		for(int dir=-1;dir<=1;dir+=2)
		{
			int np=p+dir;
			if(np<0||np>n)
				continue;

			auto relax=[&](int cash,int msk)
			{
				if(cash>=mxc||cw+1>=dist_[np][cash][msk])
					return;
				dist_[np][cash][msk]=cw+1;
				Q.push({cw+1,np,cash,msk});
			};
			relax(c,mask);                        // 不结算
			if(np>=1)
			{
				int i=np,b=1<<(i-1);
				if(mask&b)
					continue;
				if(d[i]>0)
				{
					relax(c+d[i],mask|b);         // 收钱
				}
				else if(c>=-d[i])
				{
					relax(c+d[i],mask|b);         // 还钱
				}
			}
		}
	}
	cout<<"NO\n";
	return 0;
}
