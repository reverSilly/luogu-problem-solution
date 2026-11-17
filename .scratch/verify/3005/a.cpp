// 题目：P3005 [USACO10DEC] The Trough Game S
// N<=20 ⇒ 枚举全部 2^N 个 01 状态，用 bitmask+popcount 表 O(1) 核对每条询问
// 只需区分 0/1/>=2 个解，找到第 2 个解立即退出
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25},mxm{105},mxh{65536};

int n,m,t[mxm],msk[mxm],pc[mxh];
int cnt,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=1;i<mxh;++i)
		pc[i]=pc[i>>1]+(i&1);	// pc[i]=i 的二进制 1 个数，pc[0]=0 靠全局默认
	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		string s;
		cin>>s>>t[i];
		for(int j=0;j<n;++j)
			if(s[j]=='1')
				msk[i]|=1<<j;	// 字符串第 j 位 ↔ 整数第 j 位，输出同序
	}
	for(int s=0;s<(1<<n);++s)
	{
		bool ok=true;
		for(int i=0;i<m;++i)
		{
			int x=s&msk[i];
			if(pc[x&65535]+pc[x>>16]!=t[i])
			{ok=false;break;}
		}
		if(ok)
		{
			++cnt;ans=s;
			if(cnt>=2)
				break;	// 已能断定 NOT UNIQUE，不必数完
		}
	}
	if(cnt==0)
		cout<<"IMPOSSIBLE\n";
	else if(cnt>=2)
		cout<<"NOT UNIQUE\n";
	else
	{
		for(int j=0;j<n;++j)
			cout<<((ans>>j)&1);
		cout<<'\n';
	}
	return 0;
}
