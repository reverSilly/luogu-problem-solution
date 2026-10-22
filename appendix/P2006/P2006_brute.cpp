// 题目：P2006 赵神牛的游戏
// 暴力：逐次施放模拟，每个技能重复扣法力累加伤害
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{30005};

int m;
i64 k,n;
i64 a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>m>>n;
	for(int i=0;i<m;++i)
		cin>>a[i]>>b[i];
	bool first=true;
	for(int i=0;i<m;++i)
	{
		i64 mana=k,dmg=0;
		//逐次施放，最多 100000 次（题目保证 k<=3e4）
		for(int t=0;t<100000;++t)
		{
			if(a[i]>0)
			{
				if(mana<a[i])
					break;
				mana-=a[i];
			}
			dmg+=b[i];
			if(dmg>=n)
				break;
		}
		if(dmg>=n)
		{
			if(!first)
				cout<<' ';
			first=false;
			cout<<i+1;
		}
	}
	if(first)
		cout<<-1;
	cout<<'\n';
	return 0;
}
