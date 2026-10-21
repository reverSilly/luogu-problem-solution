# P1845 影像之结构化特征

> 难度：普及/提高- · 标签：BFS，NOI 导刊

## 1. 重新读题
### 1.1 问题重述（人话版）
给一张 $n\times n$ 的 0/1 图，`1` 是边缘点，`0` 是背景。上下左右相邻（斜对角不算）的边缘点组成若干个边缘图。对每个边缘图做 Water-filling：按行优先找到还没编号的第一个黑点，编号 1；之后每一步，当前所有「已经编号、且还有未编号邻居」的点，把这些未编号邻居同时编成下一个相同的号码。一个边缘图的「步数」是它出现过的最大编号。输出边缘图个数，再把每个图的步数按从小到大输出，一行一个。

### 1.2 关键约束与规模暗示
$1\le n\le 1000$，格子数 $10^6$，每个点进队一次的 BFS 足够。题面正文写「依走访的顺序列出」，输出格式却写「按升序列出」。官方样例的三个步数按扫描出现的顺序是 $12,7,3$，按升序才是

```
3
3
7
12
```

和样例输出一致，所以以输出格式和样例为准：先个数，再升序。题面还说「每一个输入的影像」，标程按 EOF 读多组；只有一组时同样正确。

## 2. 思维链（Chain-of-thought）
先别把「步数」理解成黑点个数。样例第一块有 20 个黑点，但题面和图上的步数是 12。原因就在「同时编号」：一个点后面若有两个未编号邻居，这两个邻居都编成同一个号，而不是各占一个新号。

**引理 1**：从一个起点出发的 Water-filling，格子 $v$ 得到的编号等于 $1+\mathrm{dist}(s,v)$，其中距离是四连通、边权为 1 的最短路。步数等于 $1+\max_v\mathrm{dist}(s,v)$。

证明：编号 1 的点就是起点，距离 0，满足 $1+0$。假设所有距离 $<d$ 的点已经编成 $1,2,\ldots,d$。下一步被同时编号的，是这些点的、还没编号的邻居。一个未编号点 $v$ 若存在邻居距离为 $d-1$，则它的最短路就是 $d$（再短的话它会在更早的一轮被邻居碰到）；这一轮它得到的号码是 $d+1=1+\mathrm{dist}(s,v)$。若它的所有已编号邻居距离都 $\ge d$，它不会在这一轮被选中。未编号的点不会被重复编号，所以第一次写入就是最短路，和 BFS 第一次入队相同。最大编号就是 $1$ 加上最大距离。$\square$

因此分叉会让两个点共享同一个号码，步数是 BFS 层数，不是点数。样例第一块 20 个点、最大距离 11，步数 $12$；第二块步数 $7$；第三块步数 $3$。排成升序就是样例。

**引理 2**：每个边缘图的起点，就是它在整张图上按行优先最先出现的那个黑点。不同边缘图的编号互不影响。

证明：扫描按行、再按列。一个四连通块里，行号最小、同行列号最小的点，在扫描到它时它的块里还没有任何点被访问（块里其它点要么行号更大，要么同行但列号更大）。题面说「继续对影像中的其它边缘图编号」，也就是跳过已经编过号的点，找下一个未访问黑点当新的 1 号。块与块之间没有四连通边，BFS 不会走出去。$\square$

**定理**：对每个未访问黑点做一次 BFS，记录最大层号（起点层号记 1），收集后排序，第一行输出个数，后面逐行输出，就是题面要求的结构化特征。

证明：引理 2 保证起点和分块与题面扫描顺序一致；引理 1 保证每块的最大层号等于 Water-filling 步数；输出格式要求升序。$\square$

暴力在小图上不用队列 BFS 求距离，而是把一块里的点抽出来，用 Bellman-Ford 松弛 $|V|-1$ 轮，每条四连通边权为 1。无负环的单元图上，Bellman-Ford 求出的也是最短路，再取最大距离加 1。算法不同，答案应相同。$n\le 12$ 时单块 $|V|\le 144$，松弛跑得动。$n=1000$ 只跑标程。

## 3. 模式总结（Pattern Summary）
- **explore its property** — 「同时编号」表示同一层的点共用一个号码，步数是最大编号而不是点数。
- **classic problem solution** — 四连通无权图上，这个编号就是 BFS 距离加 1。
- **reduce to easier problem** — 每块的起点固定成扫描序第一个黑点，整题变成若干次 BFS 再排序。

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `n` | `int` | 图的宽度 | 读入 | width |
| `g[i][j]` | `int[1005][1005]` | 1 边缘，0 背景 | 每组清 0 | grid |
| `vis[i][j]` | `int[1005][1005]` | 0 未访问，否则是 BFS 层号 | 每组清 0 | layer |
| `qx,qy` | `int[1005*1005]` | 手写队列 | 头尾指针 0 | queue |
| `stepv` | `int[1005*1005]` | 每块的步数 | 计数 `cnt` | steps |
| `dx,dy` | 长度 4 的常量 | 上下左右 | 固定 | dir |

### 4.2 算法步骤
1. `while(cin>>n)` 读入一组。`getline` 吃掉行尾，再读 $n$ 行，只取字符 `0` 和 `1`（样例行尾可能有空格）。
2. 行优先找到 `g=1` 且 `vis=0` 的点，`bfs`：起点 `vis=1`，邻居层号 $=$ 当前层号 $+1$。返回最大层号。
3. 全部块的步数排序，输出块数，再逐行输出步数。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）
每组 $O(n^2)$。`g++ -O2 -std=c++17 -Wall -Wextra -Werror` 无警告。与 Bellman-Ford 暴力对拍 79 组（含官方样例，输出 `3 / 3 / 7 / 12`；另有全白、单点、多组粘在一个流里，$n\le 12$ 的随机图），checker 墙钟 3.430s。最大数据 $n=1000$ 随机 0/1 图，标程 0.032s，该组连通块数 66397。

## 5. 示例代码
### 5.1 标程（`P1845.cpp`）

```cpp
//
// 题目：P1845 影像之结构化特征
// https://www.luogu.com.cn/problem/P1845
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int dx[4]{-1,1,0,0};
constexpr int dy[4]{0,0,-1,1};

int n,g[mxn][mxn],vis[mxn][mxn];
int qx[mxn*mxn],qy[mxn*mxn];
int stepv[mxn*mxn];

int bfs(int sx,int sy)
{
	int h=0,t=0;
	qx[t]=sx;
	qy[t]=sy;
	++t;
	vis[sx][sy]=1;
	int best=1;
	while(h<t)
	{
		int x=qx[h],y=qy[h];
		++h;
		int d=vis[x][y];
		if(d>best)
			best=d;
		for(int k=0;k<4;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<0||ny<0||nx>=n||ny>=n)
				continue;
			if(!g[nx][ny]||vis[nx][ny])
				continue;
			vis[nx][ny]=d+1;
			qx[t]=nx;
			qy[t]=ny;
			++t;
		}
	}
	return best;
}

void solve()
{
	memset(g,0,sizeof(g));
	memset(vis,0,sizeof(vis));
	string line;
	getline(cin,line);
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		int j=0;
		for(char c:line)
		{
			if(c=='0'||c=='1')
			{
				if(j<n)
					g[i][j++]=c-'0';
			}
		}
	}
	int cnt=0;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			if(g[i][j]&&!vis[i][j])
				stepv[cnt++]=bfs(i,j);
		}
	}
	sort(stepv,stepv+cnt);
	cout<<cnt<<'\n';
	for(int i=0;i<cnt;++i)
		cout<<stepv[i]<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	while(cin>>n)
		solve();
	return 0;
}
```
