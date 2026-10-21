//
// 题目：P1630 求和
// https://www.luogu.com.cn/problem/P1630
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int MOD{10000};

int qpow(int a,int b)
{
	int r=1%MOD;
	for(;b;b>>=1,a=a*a%MOD)
		if(b&1)
			r=r*a%MOD;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		i64 a,b;
		cin>>a>>b;
		i64 q=a/MOD;
		int r=a%MOD;
		int blk=0,pre=0;                // blk:一整块 1..10000 的和；pre:1..r 的和
		for(int i=1;i<=MOD;++i)
		{
			int x=qpow(i%MOD,(int)b);
			blk+=x;
			if(blk>=MOD)
				blk-=MOD;
			if(i==r)
				pre=blk;                // 此时 blk 恰为前 r 项之和
		}
		int ans=(int)((q%MOD)*blk%MOD+pre)%MOD;
		cout<<ans<<'\n';
	}
	return 0;
}
