//
// 题目：P1526 [NOI2003] 智破连环阵
// https://www.luogu.com.cn/problem/P1526
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};           // M,n<=100 (+5 松弛)
constexpr int inf{1145141919};

int m,n,k;
int wx[mxn],wy[mxn],bx[mxn],by[mxn];
bool cov[mxn][mxn];               // cov[b][w]=炸弹 b 能否覆盖武器 w
int rch[mxn][mxn];                // rch[b][i]=炸弹 b 从武器 i 开始连续可炸毁到的最远武器下标(不可炸 i 则为 i-1)
int availCnt[mxn];                // availCnt[w]=当前未使用炸弹中能覆盖武器 w 的数量
int h[mxn];                       // h[i]=不限制炸弹复用时从 i 到 m 的最少步数(IDA* 静态可采纳下界)
bool used[mxn];
int lim;

bool inRange(int b,int w)
{
	i64 dx=bx[b]-wx[w],dy=by[b]-wy[w];
	return dx*dx+dy*dy<=1LL*k*k;
}

bool ida(int cur,int dep,bitset<mxn>&conflict)
{
	if(cur==m)
		return true;
	if(dep+h[cur]>lim)
		return false;

	// 前瞻死点检测: 若后缀 [cur, m-1] 中某武器 w 已无任何可用炸弹覆盖, 记录所有能覆盖 w 的炸弹为冲突源并立即回溯
	for(int w=cur;w<m;++w)
		if(availCnt[w]==0)
		{
			for(int b=0;b<n;++b)
				if(cov[b][w])
					conflict.set(b);
			return false;
		}

	pair<int,int>cands[mxn];
	int cnt=0;
	for(int b=0;b<n;++b)
		if(rch[b][cur]>=cur)
		{
			int r=rch[b][cur];
			if(dep+1+h[r+1]<=lim)
			{
				if(used[b])
					conflict.set(b);      // 记录因被祖先占用而无法在 cur 处使用的合法炸弹
				else
					cands[cnt++]={r,b};
			}
		}
	sort(cands,cands+cnt,greater<>());    // 按右端点 r 从大到小排序

	for(int idx=0;idx<cnt;++idx)
	{
		auto[r,b]=cands[idx];
		used[b]=true;
		for(int w=0;w<m;++w)
			if(cov[b][w])
				--availCnt[w];

		bitset<mxn>subConf;
		if(ida(r+1,dep+1,subConf))
			return true;

		for(int w=0;w<m;++w)
			if(cov[b][w])
				++availCnt[w];
		used[b]=false;
		conflict|=subConf;

		// 单调性与冲突导向回跳定理:
		// 若 (r+1, dep+1) 子树失败且全程未受 used[b] 限制, 换用任何 r'<=r 的其它炸弹 b' 必同样失败!
		if(!subConf.test(b))
			return false;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>m>>n>>k))
		return 0;
	for(int i=0;i<m;++i)
		cin>>wx[i]>>wy[i];
	for(int i=0;i<n;++i)
		cin>>bx[i]>>by[i];

	for(int b=0;b<n;++b)
	{
		rch[b][m]=m-1;
		for(int i=m-1;i>=0;--i)
		{
			cov[b][i]=inRange(b,i);
			if(cov[b][i])
			{
				++availCnt[i];
				rch[b][i]=max(i,rch[b][i+1]);
			}
			else
				rch[b][i]=i-1;
		}
	}

	h[m]=0;
	for(int i=m-1;i>=0;--i)
	{
		h[i]=inf;
		for(int b=0;b<n;++b)
			if(rch[b][i]>=i&&h[rch[b][i]+1]<inf)
				h[i]=min(h[i],1+h[rch[b][i]+1]);
	}

	for(lim=h[0];lim<=n;++lim)
	{
		bitset<mxn>conf;
		if(ida(0,0,conf))
		{
			cout<<lim<<'\n';
			return 0;
		}
	}
	return 0;
}
