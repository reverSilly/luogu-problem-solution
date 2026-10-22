// 题目：P2001 硬币的面值
// 贪心：始终使用「当前能用的最大面值」，可用乘法批量统计次数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using u64=unsigned long long;
using u128=__uint128_t;

constexpr int mxn{200005};

int n;
u64 m;
u64 a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	//去重，并丢弃大于 m 的面值（面值 > m 的硬币永远用不上）
	int k=0;
	for(int i=0;i<n;++i)
	{
		if(a[i]>m)
			break;
		if(k==0||a[k-1]!=a[i])
			a[k++]=a[i];
	}
	//拼不出 1 就无解
	if(k==0||a[0]!=1)
	{
		cout<<"No answer!!!\n";
		return 0;
	}
	u128 cov=0;//当前能连续拼出的区间 [0,cov]
	u64 cnt=0;//已选的硬币数
	for(int i=0;i<k;++i)
	{
		u64 v=a[i];
		//下一个面值变成「可用」的门槛：cov >= a[i+1]-1
		u128 target=(i+1<k)?(u128)(a[i+1]-1):(u128)m;
		if(cov>=target)
			continue;//a[i+1] 已经可用，贪心直接换更大的面值
		u128 need=target-cov;
		u128 times=(need+v-1)/v;
		//一旦拼满 m 就停手
		if(cov<(u128)m)
		{
			u128 tm=((u128)m-cov+v-1)/v;
			if(tm<times)
				times=tm;
		}
		cov+=(u128)times*v;
		cnt+=(u64)times;
		if(cov>=(u128)m)
			break;
	}
	cout<<cnt<<'\n';
	return 0;
}
