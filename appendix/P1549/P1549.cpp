//
// 题目：P1549 [NOIP 1997 提高组] 棋盘问题
// https://www.luogu.com.cn/problem/P1549
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{12};
constexpr int mxv{205};

int n,best_sum;
bool is_p[mxv],used[mxv];
int g[mxn][mxn],best_g[mxn][mxn];
vector<int> adj_p[mxv];
vector<pair<int,int>> cells;

void dfs(int idx,int cur_sum)
{
	if(cur_sum>=best_sum)
		return;
	if(idx==static_cast<int>(cells.size()))
	{
		best_sum=cur_sum;
		memcpy(best_g,g,sizeof(g));
		return;
	}
	auto [r,c]=cells[idx];

	// 乐观下界剪枝: 统计剩余位于第一行或第一列的空位数 rem_rc
	int rem_rc=0;
	for(int k=idx;k<static_cast<int>(cells.size());++k)
		if(cells[k].first==0||cells[k].second==0)
			++rem_rc;
	if(rem_rc>0)
	{
		int min_add=0,val=2;
		for(int k=0;k<rem_rc;++k)
		{
			while(val<=n*n&&used[val])
				++val;
			min_add+=val;
			++val;
		}
		if(cur_sum+min_add>=best_sum)
			return;
	}

	int prev=(r>0&&g[r-1][c])?g[r-1][c]:g[r][c-1];
	for(int v:adj_p[prev])
	{
		if(used[v])
			continue;
		if(r>0&&g[r-1][c]&&!is_p[v+g[r-1][c]])
			continue;
		if(c>0&&g[r][c-1]&&!is_p[v+g[r][c-1]])
			continue;
		used[v]=true;
		g[r][c]=v;
		int nxt_sum=cur_sum+((r==0||c==0)?v:0);
		dfs(idx+1,nxt_sum);
		g[r][c]=0;
		used[v]=false;
		// 若内部格子已将当前的 (第一行+第一列) 组合补全为合法解, 无需继续枚举该前缀的其他内部填法
		if(r>0&&c>0&&cur_sum==best_sum)
			return;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	if(n<=1||n==3)
	{
		cout<<"NO\n";
		return 0;
	}

	for(int i=2;i<mxv;++i)
	{
		is_p[i]=true;
		for(int j=2;j*j<=i;++j)
			if(i%j==0)
				is_p[i]=false;
	}
	for(int i=1;i<=n*n;++i)
		for(int j=1;j<=n*n;++j)
			if(i!=j&&is_p[i+j])
				adj_p[i].push_back(j);

	// 优先搜索第一行与第一列, 再搜索内部格子
	for(int j=1;j<n;++j)
		cells.push_back({0,j});
	for(int i=1;i<n;++i)
		cells.push_back({i,0});
	for(int i=1;i<n;++i)
		for(int j=1;j<n;++j)
			cells.push_back({i,j});

	g[0][0]=1;
	used[1]=true;
	best_sum=1e9;
	dfs(0,1);

	if(best_sum>=1e9)
	{
		cout<<"NO\n";
		return 0;
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cout<<best_g[i][j]<<(j+1==n?'\n':' ');
	return 0;
}
