// 题目：P2090 数字对
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 inf{1LL<<60};

int n;

i64 steps(i64 x,i64 y)
{
	// 反向还原：x>=y 时前驱唯一为 (x-y,y)，用欧几里得整除加速
	i64 tot=0;
	while(true)
	{
		if(y==1)
			return tot+x-1;      // (x,1) 连续减 1 共 x-1 步
		if(y==0)
			return inf;          // gcd>1，永远到不了 (1,1)
		tot+=x/y;
		i64 r=x%y;
		x=y;
		y=r;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 ans=inf;
	for(int i=1;i<=n;++i)
		ans=min(ans,steps(max((i64)n,(i64)i),min((i64)n,(i64)i)));
	if(ans==inf)
		cout<<-1<<'\n';
	else
		cout<<ans<<'\n';
	return 0;
}
