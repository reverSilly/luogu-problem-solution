//
// 题目：P1528 切蛋糕
// https://www.luogu.com.cn/problem/P1528
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{55};            // n<=50 (+5 松弛)
constexpr int mxm{1030};          // m<=1024 (+松弛)

int n,m;
int cake[mxn],rem[mxn];
int mouth[mxm];
i64 preM[mxm];                    // 嘴大小前缀和 (1-based)
i64 totCake,waste,maxWaste;

// 从大到小为第 idx (0-based) 张嘴分配蛋糕; st 为相同嘴大小时的起始蛋糕下标(对称性剪枝)
bool dfs(int idx,int st)
{
	if(idx<0)
		return true;
	for(int i=st;i<n;++i)
	{
		if(rem[i]<mouth[idx])
			continue;
		// 相同余量的蛋糕去重剪枝
		if(i>st&&rem[i]==rem[i-1])
			continue;
		rem[i]-=mouth[idx];
		int addW=(rem[i]<mouth[0]?rem[i]:0); // 若切完后该块蛋糕剩余量连最小的嘴 mouth[0] 都塞不下, 则彻底成为废料
		waste+=addW;
		if(waste<=maxWaste)
		{
			int nxtSt=(idx>0&&mouth[idx-1]==mouth[idx]?i:0);
			if(dfs(idx-1,nxtSt))
				return true;
		}
		waste-=addW;
		rem[i]+=mouth[idx];
	}
	return false;
}

bool check(int k)
{
	if(k==0)
		return true;
	if(preM[k]>totCake)
		return false;
	for(int i=0;i<n;++i)
		rem[i]=cake[i];
	waste=0;
	maxWaste=totCake-preM[k];
	return dfs(k-1,0);
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

	sort(mouth,mouth+m);
	// 丢弃连最小的嘴都满足不了的碎蛋糕, 并缩减超过总嘴数的多余人数
	int nn=0;
	totCake=0;
	for(int i=0;i<n;++i)
		if(m>0&&cake[i]>=mouth[0])
		{
			cake[nn++]=cake[i];
			totCake+=cake[i];
		}
	n=nn;
	sort(cake,cake+n,greater<>());

	for(int i=0;i<m;++i)
		preM[i+1]=preM[i]+mouth[i];
	while(m>0&&preM[m]>totCake)
		--m;

	int l=0,r=m;
	while(l<r)
	{
		int mid=(l+r+1)>>1;
		if(check(mid))
			l=mid;
		else
			r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
