//
// 题目：P1528 切蛋糕
// 暴力基准: 不假设"必选最小的 k 张嘴", 直接枚举 2^m 个人的子集, 检验该子集能否装入 n 块蛋糕(仅小 m 可用)
#include<bits/stdc++.h>
using namespace std;

int n,m;
int cake[15],rem[15],mouth[15];
vector<int>sub;

bool pack(int idx)
{
	if(idx==(int)sub.size())
		return true;
	for(int i=0;i<n;++i)
		if(rem[i]>=sub[idx])
		{
			rem[i]-=sub[idx];
			if(pack(idx+1))
				return true;
			rem[i]+=sub[idx];
		}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>cake[i];
	cin>>m;
	for(int i=0;i<m;++i)
		cin>>mouth[i];

	int ans=0;
	for(int mask=0;mask<(1<<m);++mask)
	{
		int cnt=__builtin_popcount(mask);
		if(cnt<=ans)
			continue;
		sub.clear();
		int sumM=0,sumC=0;
		for(int i=0;i<m;++i)
			if(mask>>i&1)
				sub.push_back(mouth[i]),sumM+=mouth[i];
		for(int i=0;i<n;++i)
			rem[i]=cake[i],sumC+=cake[i];
		if(sumM>sumC)
			continue;
		sort(sub.begin(),sub.end(),greater<>());
		if(pack(0))
			ans=cnt;
	}
	cout<<ans<<'\n';
	return 0;
}
