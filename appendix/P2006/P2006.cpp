// 题目：P2006 赵神牛的游戏
// 模拟：每个技能可施放 k/a_i 次，总伤害 (k/a_i)*b_i
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
		bool ok=false;
		if(a[i]==0)
		{
			//不耗蓝，理论上可无限施放：只有伤害大于 0 才能打死 boss
			ok=(b[i]>0);
		}
		else
		{
			i64 cnt=k/a[i];//最多施放次数
			i64 dmg=cnt*b[i];//总伤害，64 位防止溢出
			ok=(dmg>=n);
		}
		if(ok)
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
