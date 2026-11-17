// 题目：P2813 母舰
// 伤害全有或全无：全破防 ⇒ 伤害 = tot - 破防花费；否则 0
// 贪心：防御升序，每个防御配"剩余攻击中严格大于它的最小者"，大攻击省下来打本体
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,m;
i64 atk[mxn],def[mxn],tot;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		cin>>def[i];
	for(int i=0;i<n;++i)
	{
		cin>>atk[i];
		tot+=atk[i];
	}
	sort(def,def+m);
	sort(atk,atk+n);
	i64 used=0;
	for(int j=0,i=0;j<m;++j)
	{
		while(i<n&&atk[i]<=def[j])
			++i;	// 打不穿当前及以后所有防御，留作本体伤害（严格大于！）
		if(i==n)
		{
			cout<<0<<'\n';	// 无法全破 ⇒ 伤害为 0
			return 0;
		}
		used+=atk[i++];
	}
	cout<<tot-used<<'\n';
	return 0;
}
