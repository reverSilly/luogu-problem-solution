//
// 题目：P1585 魔法阵
// https://www.luogu.com.cn/problem/P1585
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};            // n*m<=50 (+5 松弛)
constexpr int inf{0x3f3f3f3f};
constexpr int dr[4]{1,0,-1,0};
constexpr int dc[4]{0,-1,0,1};

int n,m,k1,k2,tot,half_n,best;
int pr[mxn],pc[mxn];
bool vis[mxn][mxn];

bool valid_unvis(int r,int c)
{
	return r>=0&&r<n&&c>=0&&c<m&&!vis[r][c];
}

int count_deg(int r,int c)
{
	int cnt=0;
	for(int d=0;d<4;++d)
		if(valid_unvis(r+dr[d],c+dc[d]))
			++cnt;
	return cnt;
}

// 从 (r, c) 再走 rem 步到达某点后, 该点与目标 (tr, tc) 的加权曼哈顿距离下界
int min_possible_cost(int r,int c,int tr,int tc,int rem)
{
	int dy=abs(r-tr),dx=abs(c-tc);
	if(dy+dx<=rem)
		return 0;
	if(k1>=k2)
	{
		int take=min(dy,rem);
		dy-=take;
		rem-=take;
		dx=max(0,dx-rem);
	}
	else
	{
		int take=min(dx,rem);
		dx-=take;
		rem-=take;
		dy=max(0,dy-rem);
	}
	return dy*k1+dx*k2;
}

void dfs(int step,int r,int c,int cur_max)
{
	if(cur_max>=best)
		return;
	pr[step]=r;pc[step]=c;
	if(step>half_n)
	{
		int w=k1*abs(r-pr[step-half_n])+k2*abs(c-pc[step-half_n]);
		if(w>cur_max)
			cur_max=w;
		if(cur_max>=best)
			return;
	}
	if(step==tot)
	{
		best=cur_max;
		return;
	}

	// 1. 加权曼哈顿距离超前剪枝
	if(step<=half_n)
	{
		if(min_possible_cost(r,c,pr[1],pc[1],half_n+1-step)>=best)
			return;
		if(step>=2&&min_possible_cost(r,c,pr[step],pc[step],half_n)>=best)
			return;
	}
	else
	{
		for(int s=step+1;s<=tot;s+=2)
			if(min_possible_cost(r,c,pr[s-half_n],pc[s-half_n],s-step)>=best)
				return;
		if(min_possible_cost(r,c,pr[half_n],pc[half_n],tot-step)>=best)
			return;
	}

	// 2. 孤立点(0度邻居)必选与剪枝
	int forced_d=-1;
	for(int d=0;d<4;++d)
	{
		int nr=r+dr[d],nc=c+dc[d];
		if(valid_unvis(nr,nc))
		{
			int dg=count_deg(nr,nc);
			if(dg==0)
			{
				if(step+1<tot)
					return;
				forced_d=d;
			}
		}
	}

	// 3. 撞墙割裂剪枝 + 启发式移动排序
	int ord[4]={0,1,2,3},cost[4];
	for(int d=0;d<4;++d)
	{
		int nr=r+dr[d],nc=c+dc[d];
		if(!valid_unvis(nr,nc)||(forced_d!=-1&&d!=forced_d))
		{
			cost[d]=inf;
			continue;
		}
		int fd_r=nr+dr[d],fd_c=nc+dc[d];
		int d1=(d+1)&3,d2=(d+3)&3;
		if(!valid_unvis(fd_r,fd_c)&&valid_unvis(nr+dr[d1],nc+dc[d1])&&valid_unvis(nr+dr[d2],nc+dc[d2]))
		{
			cost[d]=inf;
			continue;
		}
		if(step+1>half_n)
			cost[d]=k1*abs(nr-pr[step+1-half_n])+k2*abs(nc-pc[step+1-half_n]);
		else
		{
			int dir_w=(dr[d]!=0)?k1:k2;
			cost[d]=count_deg(nr,nc)*100000+dir_w;
		}
	}
	sort(ord,ord+4,[&](int a,int b){return cost[a]<cost[b];});

	for(int idx=0;idx<4;++idx)
	{
		int d=ord[idx];
		if(cost[d]>=inf)
			break;
		int nr=r+dr[d],nc=c+dc[d];
		vis[nr][nc]=true;
		dfs(step+1,nr,nc,cur_max);
		vis[nr][nc]=false;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m>>k1>>k2))
		return 0;
	tot=n*m;
	half_n=tot>>1;
	best=inf;
	vis[0][m-1]=true;
	dfs(1,0,m-1,0);
	cout<<best<<'\n';
	return 0;
}
